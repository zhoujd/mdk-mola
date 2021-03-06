/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)matrix_2003.c
 */

#include "app_base.h"
#include "matrix_2003.h"
#include "task_base.h"

// Blend State
static VABlendState blend_state = { /** \brief Video blending flags. */
    .flags        = VA_BLEND_PREMULTIPLIED_ALPHA, //VA_BLEND_GLOBAL_ALPHA,
    .global_alpha = 1,
    .min_luma     = 0,
    .max_luma     = 1
};

static zzStatus ZZMatrix2003_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix2003_PartStart(zzMatrixBaseST *pSelf);

static zzStatus ZZMatrix2003_ProcNextFrame(zzMatrix2003ST  *pSelf);
static zzStatus ZZMatrix2003_ParseInputString(zzMatrix2003ST  *pSelf, int nArgNum, char **strInput);
static zzStatus ZZMatrix2003_ParseVpString(zzMatrix2003ST  *pSelf, int nArgNum, char **strInput);


//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,      NULL, ZZMatrix2003_Start,      NULL},
    {ZZ_EVENT_PART_START, NULL, ZZMatrix2003_PartStart,  NULL},
};


zzStatus ZZMatrix2003_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 2003 help\n");

    return sts;
}

zzStatus ZZMatrix2003_HelpRegister()
{
    zzStatus  sts    = ZZ_ERR_NONE;
    zzAppST   *pApp  = GetAPP();

    zzHelpInfoST  *pHelpInfo = NULL;

    pHelpInfo = (zzHelpInfoST *)AllocAndZeroMem(sizeof(zzHelpInfoST));
    if (pHelpInfo == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    pHelpInfo->help_id         = MATRIX2003_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix2003_Help;

    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix2003_Create(zzMatrix2003ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;

    *ppRet = (zzMatrix2003ST *)AllocAndZeroMem(sizeof(zzMatrix2003ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);

    (*ppRet)->base.matrix_id     = MATRIX2003_ID;

END:
    return sts;
}

zzStatus ZZMatrix2003_Release(zzMatrix2003ST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;
    int       i   = 0;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    for (i = 0; i < pSelf->numFilterBufs; i++)
    {
        vaDestroyBuffer(pSelf->ctx->va_dpy, pSelf->filterBufs[i]);
        pSelf->filterBufs[i] =  VA_INVALID_ID;
    }

    return sts;
}


zzStatus ZZMatrix2003_Init(zzMatrix2003ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl, zzVAContextST *pCtx,
                           zzSurfaceST    *pSrcSurf, zzSurfaceST *pDstSurf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    pSelf->ctx             = pCtx;
    pSelf->base.pipe_ctrl  = pPipeCtrl;
    pSelf->src_surf        = *pSrcSurf;
    pSelf->dst_surf        = *pDstSurf;
    pSelf->pipelineParamID = VA_INVALID_ID;

    sts = ZZMatrix2003_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2003_ParseInputString error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix2003_Start(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix2003ST *pSelf = GET_MATRIX2003(pMatrixBase);


    ZZDEBUG("Matrix %d Start\n", pSelf->base.matrix_id);

    if (pSelf->base.pipe_ctrl->pipe_event == ZZ_EVENT_PIPE_EXIT)
    {
        ZZDEBUG("(%s) get exist event\n", __FUNCTION__);
        pMatrixBase->next_event = ZZ_EVENT_END;
        goto END;
    }

    sts = ZZMatrix2003_ProcNextFrame(pSelf);
    switch (sts)
    {
    case ZZ_ERR_NONE:
        pMatrixBase->next_event = ZZ_EVENT_PART_END;
        break;
    default:
        ZZPRINTF("ZZMatrix2003_ProcNextFrame error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix2003_PartStart(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix2003ST *pSelf = GET_MATRIX2003(pMatrixBase);

    ZZDEBUG("Matrix %d PartStart\n", pSelf->base.matrix_id);

    if (pSelf->base.pipe_ctrl->pipe_event == ZZ_EVENT_PIPE_EXIT)
    {
        ZZDEBUG("(%s) get exist event\n", __FUNCTION__);
        pMatrixBase->next_event = ZZ_EVENT_END;
        goto END;
    }

    sts = ZZMatrix2003_ProcNextFrame(pSelf);
    switch (sts)
    {
    case ZZ_ERR_NONE:
        pMatrixBase->next_event = ZZ_EVENT_PART_END;
        break;
    default:
        ZZPRINTF("ZZMatrix2003_ProcNextFrame error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix2003_ProcNextFrame(zzMatrix2003ST  *pSelf)
{
    zzStatus    sts = ZZ_ERR_NONE;
    VAStatus    vaSts;

    VASurfaceID* srf = &pSelf->src_surf.id;
    pSelf->pipelineParam.surface = *srf;

    VARectangle srcRect;
    srcRect.x = pSelf->src_surf.frameInfo.CropX;
    srcRect.y = pSelf->src_surf.frameInfo.CropY;
    srcRect.width = pSelf->src_surf.frameInfo.CropW;
    srcRect.height= pSelf->src_surf.frameInfo.CropH;
    pSelf->pipelineParam.surface_region = &srcRect;

    VARectangle dstRect;
    dstRect.x = pSelf->dst_surf.frameInfo.CropX;
    dstRect.y = pSelf->dst_surf.frameInfo.CropY;
    dstRect.width = pSelf->dst_surf.frameInfo.CropW;
    dstRect.height= pSelf->dst_surf.frameInfo.CropH;
    pSelf->pipelineParam.output_region = &dstRect;

    pSelf->pipelineParam.output_background_color = 0xff108080; // black

    switch (pSelf->params.rota_angle)
    {
    case VA_ROTATION_NONE:
    case VA_ROTATION_90:
    case VA_ROTATION_180:
    case VA_ROTATION_270:
        pSelf->pipelineParam.rotation_state = pSelf->params.rota_angle; // rotation
        break;
    default:
        ZZPRINTF("ZZMatrix2003_ProcNextFrame rota_angle unsupport %d\n", pSelf->params.rota_angle);
        sts = ZZ_ERR_UNSUPPORTED;
        goto END;
    }

    zzU32  refFourcc = pSelf->src_surf.frameInfo.FourCC;
    switch (refFourcc)
    {
    case ZZ_FOURCC_ABGR:
    case ZZ_FOURCC_ARGB:
    case ZZ_FOURCC_XRGB:
    case ZZ_FOURCC_R5G6B5:
        pSelf->pipelineParam.surface_color_standard = VAProcColorStandardNone;
        break;
    case ZZ_FOURCC_NV12:
    case ZZ_FOURCC_YV12:
    case ZZ_FOURCC_YUY2:
    case ZZ_FOURCC_400P:
    case ZZ_FOURCC_411P:
    case ZZ_FOURCC_IMC3:
    case ZZ_FOURCC_422H:
    case ZZ_FOURCC_422V:
    case ZZ_FOURCC_444P:
        pSelf->pipelineParam.surface_color_standard = VAProcColorStandardBT601;
        break;
    case ZZ_FOURCC_P010:
        pSelf->pipelineParam.surface_color_standard = VAProcColorStandardExplicit;
        break;
    case ZZ_FOURCC_A2RGB10:
        pSelf->pipelineParam.surface_color_standard = VAProcColorStandardExplicit;
        break;
    default:
        ZZPRINTF("ZZMatrix2003_ProcNextFrame target unsupport %d\n", refFourcc);
        sts = ZZ_ERR_UNSUPPORTED;
        goto END;

    }

    zzU32  targetFourcc = pSelf->dst_surf.frameInfo.FourCC;
    switch (targetFourcc)
    {
    case ZZ_FOURCC_ABGR:
    case ZZ_FOURCC_ARGB:
    case ZZ_FOURCC_XRGB:
    case ZZ_FOURCC_R5G6B5:
        pSelf->pipelineParam.output_color_standard = VAProcColorStandardNone;
        break;
    case ZZ_FOURCC_NV12:
    case ZZ_FOURCC_YV12:
    case ZZ_FOURCC_YUY2:
        pSelf->pipelineParam.output_color_standard = VAProcColorStandardBT601;
        break;
    case ZZ_FOURCC_P010:
        pSelf->pipelineParam.output_color_standard = VAProcColorStandardExplicit;
        break;
    case ZZ_FOURCC_A2RGB10:
        pSelf->pipelineParam.output_color_standard = VAProcColorStandardExplicit;
        break;
    default:
        ZZPRINTF("ZZMatrix2003_ProcNextFrame target unsupport %d\n", targetFourcc);
        sts = ZZ_ERR_UNSUPPORTED;
        goto END;
    }

    ZZPRINTF("IN_C=%d, OUT_C=%d\n", pSelf->pipelineParam.surface_color_standard, pSelf->pipelineParam.output_color_standard);

    switch (pSelf->params.hdr_type)
    {
    case VA_TONE_MAPPING_HDR_TO_HDR:
        ZZDEBUG("H2H setting\n");
        pSelf->pipelineParam.surface_color_standard = VAProcColorStandardExplicit;
        pSelf->pipelineParam.output_color_standard = VAProcColorStandardExplicit;
        pSelf->pipelineParam.input_color_properties.colour_primaries = 9;
        pSelf->pipelineParam.output_color_properties.colour_primaries = 9;
        pSelf->pipelineParam.input_color_properties.transfer_characteristics = 16;
        pSelf->pipelineParam.output_color_properties.transfer_characteristics = 16;
        break;
    case VA_TONE_MAPPING_HDR_TO_SDR:
        ZZDEBUG("H2S setting\n");
        pSelf->pipelineParam.surface_color_standard = VAProcColorStandardExplicit;
        pSelf->pipelineParam.input_color_properties.colour_primaries = 9;
        pSelf->pipelineParam.input_color_properties.transfer_characteristics = 16;
        break;
    default:
        break;
    }

    switch (pSelf->src_surf.frameInfo.PicStruct)
    {
    case ZZ_PICSTRUCT_PROGRESSIVE:
        pSelf->pipelineParam.filter_flags = VA_FRAME_PICTURE;
        break;
    case ZZ_PICSTRUCT_FIELD_TFF:
        pSelf->pipelineParam.filter_flags = VA_TOP_FIELD | VA_FILTER_SCALING_FAST;
        break;
    case ZZ_PICSTRUCT_FIELD_BFF:
        pSelf->pipelineParam.filter_flags = VA_BOTTOM_FIELD | VA_FILTER_SCALING_FAST;
        break;
    }

    pSelf->pipelineParam.filters  = pSelf->filterBufs;
    pSelf->pipelineParam.num_filters  = pSelf->numFilterBufs;

    if (pSelf->params.hdr_type == VA_TONE_MAPPING_HDR_TO_HDR)
    {
        ZZPRINTF("HDR test ...\n");

        ZERO_MEMORY(pSelf->out_metadata);
        ZERO_MEMORY(pSelf->out_hdr_metadata);

        // The output is HDR content
#if 1 //zhoujd
        zzMatrix2003VpParamsST *pParam = &pSelf->params.vp_params;
        pSelf->out_metadata.max_display_mastering_luminance = pParam->hdr_output.max_display_mastering_luminance;
        pSelf->out_metadata.min_display_mastering_luminance = pParam->hdr_output.min_display_mastering_luminance;
        pSelf->out_metadata.max_content_light_level         = pParam->hdr_output.max_content_light_level;
        pSelf->out_metadata.max_pic_average_light_level     = pParam->hdr_output.max_pic_average_light_level;
        pSelf->out_metadata.display_primaries_x[0] = pParam->hdr_output.display_primaries_x[0];
        pSelf->out_metadata.display_primaries_x[1] = pParam->hdr_output.display_primaries_x[1];
        pSelf->out_metadata.display_primaries_x[2] = pParam->hdr_output.display_primaries_x[2];
        pSelf->out_metadata.display_primaries_y[0] = pParam->hdr_output.display_primaries_y[0];
        pSelf->out_metadata.display_primaries_y[1] = pParam->hdr_output.display_primaries_y[1];
        pSelf->out_metadata.display_primaries_y[2] = pParam->hdr_output.display_primaries_y[2];
        pSelf->out_metadata.white_point_x = pParam->hdr_output.white_point_x;
        pSelf->out_metadata.white_point_y = pParam->hdr_output.white_point_y;
#else
        pSelf->out_metadata.max_display_mastering_luminance = 1000;
        pSelf->out_metadata.min_display_mastering_luminance = 1;
        pSelf->out_metadata.max_content_light_level         = 4000;
        pSelf->out_metadata.max_pic_average_light_level     = 1000;
        pSelf->out_metadata.display_primaries_x[0] = 8500;
        pSelf->out_metadata.display_primaries_y[0] = 39850;
        pSelf->out_metadata.display_primaries_x[1] = 35400;
        pSelf->out_metadata.display_primaries_y[1] = 14600;
        pSelf->out_metadata.display_primaries_x[2] = 6550;
        pSelf->out_metadata.display_primaries_y[2] = 2300;
        pSelf->out_metadata.white_point_x = 15635;
        pSelf->out_metadata.white_point_y = 16450;
#endif //zhoujd

        pSelf->out_hdr_metadata.metadata_type = VAProcHighDynamicRangeMetadataHDR10;
        pSelf->out_hdr_metadata.metadata      = &pSelf->out_metadata;
        pSelf->out_hdr_metadata.metadata_size = sizeof(VAHdrMetaDataHDR10);

        pSelf->pipelineParam.output_hdr_metadata = &pSelf->out_hdr_metadata;
    }

    if ( pSelf->pipelineParamID != VA_INVALID_ID)
    {
        vaDestroyBuffer(pSelf->ctx->va_dpy, pSelf->pipelineParamID);
    }

    vaSts = vaCreateBuffer(pSelf->ctx->va_dpy,
                           pSelf->ctx->id,
                           VAProcPipelineParameterBufferType,
                           sizeof(pSelf->pipelineParam),
                           1,
                           &pSelf->pipelineParam,
                           &pSelf->pipelineParamID);
    sts     = va_to_zz_status(vaSts);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateBuffer error\n");
        goto END;
    }


    if (pSelf->bCompFlag == TRUE)
    {
        //subpic setting
        pSelf->subpicParam = pSelf->pipelineParam;
        pSelf->subpicParam.pipeline_flags |= VA_PROC_PIPELINE_FAST;
        pSelf->subpicParam.filter_flags |= VA_FILTER_SCALING_FAST;

        blend_state.global_alpha = pSelf->params.vp_params.composition.comp_alpha;
        pSelf->subpicParam.blend_state = &blend_state;

        VARectangle dstSubRect;
        dstSubRect.x = pSelf->dst_surf.frameInfo.CropX;
        dstSubRect.y = pSelf->dst_surf.frameInfo.CropY;
        dstSubRect.width = pSelf->dst_surf.frameInfo.CropW / 2;
        dstSubRect.height= pSelf->dst_surf.frameInfo.CropH / 2;
        pSelf->subpicParam.output_region = &dstSubRect;

        vaSts = vaCreateBuffer(pSelf->ctx->va_dpy,
                               pSelf->ctx->id,
                               VAProcPipelineParameterBufferType,
                               sizeof(pSelf->subpicParam),
                               1,
                               &pSelf->subpicParam,
                               &pSelf->subpicParamID);
        sts     = va_to_zz_status(vaSts);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("vaCreateBuffer error\n");
            goto END;
        }
    }

    VASurfaceID *outputSurface = &pSelf->dst_surf.id;
    vaSts = vaBeginPicture(pSelf->ctx->va_dpy, pSelf->ctx->id, *outputSurface);
    sts     = va_to_zz_status(vaSts);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaBeginPicture error\n");
        goto END;
    }


    vaSts = vaRenderPicture(pSelf->ctx->va_dpy, pSelf->ctx->id, &pSelf->pipelineParamID, 1);
    sts     = va_to_zz_status(vaSts);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaRenderPicture error\n");
        goto END;
    }

    if (pSelf->bCompFlag == TRUE)
    {
        vaSts = vaRenderPicture(pSelf->ctx->va_dpy, pSelf->ctx->id, &pSelf->subpicParamID, 1);
        sts     = va_to_zz_status(vaSts);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("vaRenderPicture error\n");
            goto END;
        }
    }

    vaSts = vaEndPicture(pSelf->ctx->va_dpy, pSelf->ctx->id);
    sts     = va_to_zz_status(vaSts);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaEndPicture error\n");
        goto END;
    }

END:
    if (pSelf->pipelineParamID != VA_INVALID_ID)
    {
        vaDestroyBuffer(pSelf->ctx->va_dpy, pSelf->pipelineParamID);
        pSelf->pipelineParamID = VA_INVALID_ID;
    }

    if (pSelf->bCompFlag == TRUE)
    {
        if (pSelf->subpicParamID != VA_INVALID_ID)
        {
            vaDestroyBuffer(pSelf->ctx->va_dpy, pSelf->subpicParamID);
            pSelf->subpicParamID = VA_INVALID_ID;
        }
    }

    return sts;
}

zzStatus ZZMatrix2003_ParseInputString(zzMatrix2003ST *pSelf, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;
    zzU8               i       = 1;

    for (i = 1; i < nArgNum; i++ )
    {
        CHECK_POINTER(strInput[i], ZZ_ERR_NULL_PTR);
        {
            if ( 0 == zz_strcmp(strInput[i], ZZ_STRING("-angle")) )
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%d"), &pSelf->params.rota_angle);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-h2h")))
            {
                pSelf->params.hdr_type = VA_TONE_MAPPING_HDR_TO_HDR;
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-h2s")))
            {
                pSelf->params.hdr_type = VA_TONE_MAPPING_HDR_TO_SDR;
            }
        }
    }

    sts =  ZZMatrix2003_ParseVpString(pSelf, nArgNum, strInput);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2003_ParseVpString error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix2003_ParseVpString(zzMatrix2003ST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;
    zzU8               i       = 1;

    zzMatrix2003VpParamsST  *pParams = &pSelf->params.vp_params;

    for (i = 1; i < nArgNum; i++ )
    {
        CHECK_POINTER(strInput[i], ZZ_ERR_NULL_PTR);
        {
            if ( 0 == zz_strcmp(strInput[i], ZZ_STRING("--sharpness")) )
            {
                VABufferID  sharp_buf_id = VA_INVALID_ID;

                VAL_CHECK(1 + i == nArgNum);
                i++;

                sts = render_parser_sharpness(&pParams->sharpness, strInput[i]);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_parser_sharpness  error\n");
                    goto END;
                }

                ZZPRINTF("Matrix 2003 sharpness = %d\n", pParams->sharpness.factor);
                sts = render_picture_vp_ief(pSelf->ctx->va_dpy, pSelf->ctx->id, &sharp_buf_id, pParams->sharpness.factor);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_picture_vp_ief error\n");
                    goto END;
                }

                pSelf->filterBufs[pSelf->numFilterBufs] = sharp_buf_id;
                pSelf->numFilterBufs++;
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("--amp")))
            {
                VABufferID  amp_buf_id = VA_INVALID_ID;

                VAL_CHECK(1 + i == nArgNum);
                i++;

                sts = render_parser_amp(&pParams->amp, strInput[i]);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_parser_amp  error\n");
                    goto END;
                }

                ZZPRINTF("Matrix 2003 hue        = %f\n", pParams->amp.hue);
                ZZPRINTF("Matrix 2003 saturation = %f\n", pParams->amp.saturation);
                ZZPRINTF("Matrix 2003 brightness = %f\n", pParams->amp.brightness);
                ZZPRINTF("Matrix 2003 contrast   = %f\n", pParams->amp.contrast);

                sts = render_picture_vp_hsbc(pSelf->ctx->va_dpy, pSelf->ctx->id, &amp_buf_id, pParams->amp.hue, pParams->amp.saturation, pParams->amp.brightness, pParams->amp.contrast);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_picture_vp_hsbc error\n");
                    goto END;
                }

                pSelf->filterBufs[pSelf->numFilterBufs] = amp_buf_id;
                pSelf->numFilterBufs++;

            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("--comp")))
            {
                pSelf->bCompFlag = TRUE;
                ZZPRINTF("Matrix 2003 comp      = TRUE\n");

                VAL_CHECK(1 + i == nArgNum);
                i++;

                sts = render_parser_comp(&pParams->composition, strInput[i]);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_parser_cmp  error\n");
                    goto END;
                }

                ZZPRINTF("Matrix 2003 alpha     = %lf\n", pParams->composition.comp_alpha);
                ZZPRINTF("Matrix 2003 row       = %d\n",  pParams->composition.row_num);
                ZZPRINTF("Matrix 2003 col       = %d\n",  pParams->composition.col_num);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("--denoise")))
            {
                VABufferID  dn_buf_id = VA_INVALID_ID;

                VAL_CHECK(1 + i == nArgNum);
                i++;

                sts = render_parser_denoise(&pParams->denoise, strInput[i]);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_parser_denoise  error\n");
                    goto END;
                }

                ZZPRINTF("Matrix 2003 denoise factor   = %d\n", pParams->denoise.factor);
                sts = render_picture_vp_dn(pSelf->ctx->va_dpy, pSelf->ctx->id, &dn_buf_id, pParams->denoise.factor);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_picture_vp_dn error\n");
                    goto END;
                }

                pSelf->filterBufs[pSelf->numFilterBufs] = dn_buf_id;
                pSelf->numFilterBufs++;

            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("--deinterlace")))
            {
                VABufferID   deint_buf_id   = VA_INVALID_ID;

                VAL_CHECK(1 + i == nArgNum);
                i++;

                sts = render_parser_deinterlace(&pParams->deinterlace, strInput[i]);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_parser_deinterlace  error\n");
                    goto END;
                }

                ZZPRINTF("Matrix 2003 deinterlace algo   = %d\n", pParams->deinterlace.type);
                ZZPRINTF("Matrix 2003 deinterlace bff    = %d\n", pParams->deinterlace.bff_flag);
                ZZPRINTF("Matrix 2003 deinterlace bottom = %d\n", pParams->deinterlace.btm_field_flag);
                ZZPRINTF("Matrix 2003 deinterlace single = %d\n", pParams->deinterlace.one_field_flag);

                sts = render_picture_vp_di(pSelf->ctx->va_dpy, pSelf->ctx->id, &deint_buf_id, &pParams->deinterlace);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_picture_vp_di error\n");
                    goto END;
                }

                pSelf->filterBufs[pSelf->numFilterBufs] = deint_buf_id;
                pSelf->numFilterBufs++;
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("--hdr-input")))
            {
                VABufferID   hdr_buf_id   = VA_INVALID_ID;

                VAL_CHECK(1 + i == nArgNum);
                i++;

                sts = render_parser_hdr(&pParams->hdr_input, strInput[i]);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_parser_hdr error\n");
                    goto END;
                }

                ZZPRINTF("[M2003] hdr-input: display_primaries_x[0] = %d\n", pParams->hdr_input.display_primaries_x[0]);
                ZZPRINTF("[M2003] hdr-input: display_primaries_x[1] = %d\n", pParams->hdr_input.display_primaries_x[1]);
                ZZPRINTF("[M2003] hdr-input: display_primaries_x[2] = %d\n", pParams->hdr_input.display_primaries_x[2]);
                ZZPRINTF("[M2003] hdr-input: display_primaries_y[0] = %d\n", pParams->hdr_input.display_primaries_y[0]);
                ZZPRINTF("[M2003] hdr-input: display_primaries_y[1] = %d\n", pParams->hdr_input.display_primaries_y[1]);
                ZZPRINTF("[M2003] hdr-input: display_primaries_y[2] = %d\n", pParams->hdr_input.display_primaries_y[2]);
                ZZPRINTF("[M2003] hdr-input: white_point_x = %d\n", pParams->hdr_input.white_point_x);
                ZZPRINTF("[M2003] hdr-input: white_point_y = %d\n", pParams->hdr_input.white_point_y);
                ZZPRINTF("[M2003] hdr-input: max_display_mastering_luminance = %d\n", pParams->hdr_input.max_display_mastering_luminance);
                ZZPRINTF("[M2003] hdr-input: min_display_mastering_luminance = %d\n", pParams->hdr_input.min_display_mastering_luminance);
                ZZPRINTF("[M2003] hdr-input: max_content_light_level     = %d\n", pParams->hdr_input.max_content_light_level);
                ZZPRINTF("[M2003] hdr-input: max_pic_average_light_level = %d\n", pParams->hdr_input.max_pic_average_light_level);

                sts = render_picture_vp_hdr(pSelf->ctx->va_dpy, pSelf->ctx->id, &hdr_buf_id, &pParams->hdr_input);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_picture_vp_hdr error\n");
                    goto END;
                }

                pSelf->filterBufs[pSelf->numFilterBufs] = hdr_buf_id;
                pSelf->numFilterBufs++;
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("--hdr-output")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;

                sts = render_parser_hdr(&pParams->hdr_output, strInput[i]);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_parser_hdr error\n");
                    goto END;
                }

                ZZPRINTF("[M2003] hdr-output display_primaries_x[0] = %d\n", pParams->hdr_output.display_primaries_x[0]);
                ZZPRINTF("[M2003] hdr-output display_primaries_x[1] = %d\n", pParams->hdr_output.display_primaries_x[1]);
                ZZPRINTF("[M2003] hdr-output display_primaries_x[2] = %d\n", pParams->hdr_output.display_primaries_x[2]);
                ZZPRINTF("[M2003] hdr-output display_primaries_y[0] = %d\n", pParams->hdr_output.display_primaries_y[0]);
                ZZPRINTF("[M2003] hdr-output display_primaries_y[1] = %d\n", pParams->hdr_output.display_primaries_y[1]);
                ZZPRINTF("[M2003] hdr-output display_primaries_y[2] = %d\n", pParams->hdr_output.display_primaries_y[2]);
                ZZPRINTF("[M2003] hdr-output white_point_x = %d\n", pParams->hdr_output.white_point_x);
                ZZPRINTF("[M2003] hdr-output white_point_y = %d\n", pParams->hdr_output.white_point_y);
                ZZPRINTF("[M2003] hdr-output max_display_mastering_luminance = %d\n", pParams->hdr_output.max_display_mastering_luminance);
                ZZPRINTF("[M2003] hdr-output min_display_mastering_luminance = %d\n", pParams->hdr_output.min_display_mastering_luminance);
                ZZPRINTF("[M2003] hdr-output max_content_light_level     = %d\n", pParams->hdr_output.max_content_light_level);
                ZZPRINTF("[M2003] hdr-output max_pic_average_light_level = %d\n", pParams->hdr_output.max_pic_average_light_level);
            }
        }
    }

END:
    return sts;
}
