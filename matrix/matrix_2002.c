/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)matrix_2002.c
 */

#include "app_base.h"
#include "matrix_2002.h"
#include "task_base.h"

static zzStatus ZZMatrix2002_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix2002_Test(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix2002_End(zzMatrixBaseST *pSelf);

static zzStatus ZZMatrix2002_ParseInputString(zzMatrix2002ST  *pSelf, int nArgNum, char **strInput);
static zzStatus ZZMatrix2002_ParseVpString(zzMatrix2002ST  *pSelf, int nArgNum, char **strInput);

enum MATRIX2002_EVENT_EN
{
    ZZ_EVENT_TASK2002_TEST = ZZ_EVENT_USER + 1,
    ZZ_EVENT_TASK2002_DONE,
};

//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,         NULL, ZZMatrix2002_Start, NULL},     //ZZ_EVENT_START
    {ZZ_EVENT_TASK2002_TEST, NULL, ZZMatrix2002_Test,  NULL},     //ZZ_EVENT_TEST
    {ZZ_EVENT_TASK2002_DONE, NULL, ZZMatrix2002_End,   NULL},     //ZZ_EVENT_END
};


zzStatus ZZMatrix2002_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 2002 help\n");

    return sts;
}

zzStatus ZZMatrix2002_HelpRegister()
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

    pHelpInfo->help_id         = MATRIX2002_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix2002_Help;

    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix2002_Create(zzMatrix2002ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;

    *ppRet = (zzMatrix2002ST *)AllocAndZeroMem(sizeof(zzMatrix2002ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);

    (*ppRet)->base.matrix_id     = MATRIX2002_ID;

END:
    return sts;
}

zzStatus ZZMatrix2002_Release(zzMatrix2002ST *pSelf)
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


zzStatus ZZMatrix2002_Init(zzMatrix2002ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl, zzVAContextST *pCtx,
                           zzSurfaceST    *pSrcSurf, zzSurfaceST *pDstSurf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZMatrix2002_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2002_ParseInputString error\n");
        goto END;
    }

    pSelf->base.pipe_ctrl = pPipeCtrl;
    pSelf->ctx            = pCtx;
    pSelf->src_surf       = *pSrcSurf;
    pSelf->dst_surf       = *pDstSurf;

END:
    return sts;
}

zzStatus ZZMatrix2002_Start(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix2002ST *pSelf = GET_MATRIX2002(pMatrixBase);


    ZZPRINTF("Matrix %d Start\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_TASK2002_TEST;

    return sts;
}

zzStatus ZZMatrix2002_Test(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix2002ST *pSelf = GET_MATRIX2002(pMatrixBase);

    ZZPRINTF("Matrix %d Test\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_TASK2002_DONE;

    return sts;

}


zzStatus ZZMatrix2002_End(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix2002ST *pSelf = GET_MATRIX2002(pMatrixBase);

    ZZPRINTF("Matrix %d End\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_END;

    return sts;

}

zzStatus ZZMatrix2002_ParseInputString(zzMatrix2002ST *pSelf, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;
    zzU8               i       = 1;

    for (i = 1; i < nArgNum; i++ )
    {
        CHECK_POINTER(strInput[i], ZZ_ERR_NULL_PTR);
        {
#if ZZ_ROTATION_SUPPORT
            if ( 0 == zz_strcmp(strInput[i], ZZ_STRING("-angle")) )
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%d"), &pSelf->params.rota_angle);
            }
#endif //ZZ_ROTATION_SUPPORT
        }
    }

    sts =  ZZMatrix2002_ParseVpString(pSelf, nArgNum, strInput);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2002_ParseVpString error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix2002_ParseVpString(zzMatrix2002ST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;
    zzU8               i       = 1;

    zzMatrix2002VpParamsST  *pParams = &pSelf->params.vp_params;

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

                ZZPRINTF("Matrix 2002 sharpness = %d\n", pParams->sharpness.factor);
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

                ZZPRINTF("Matrix 2002 hue        = %f\n", pParams->amp.hue);
                ZZPRINTF("Matrix 2002 saturation = %f\n", pParams->amp.saturation);
                ZZPRINTF("Matrix 2002 brightness = %f\n", pParams->amp.brightness);
                ZZPRINTF("Matrix 2002 contrast   = %f\n", pParams->amp.contrast);

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
                ZZPRINTF("Matrix 2002 comp      = TRUE\n");

#if ZZ_BLEND_ALPHA_SUPPORT
                VAL_CHECK(1 + i == nArgNum);
                i++;

                sts = render_parser_comp(&pParams->composition, strInput[i]);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_parser_cmp  error\n");
                    goto END;
                }

                ZZPRINTF("Matrix 2002 alpha     = %lf\n", pParams->composition.comp_alpha);
                ZZPRINTF("Matrix 2002 row       = %d\n",  pParams->composition.row_num);
                ZZPRINTF("Matrix 2002 col       = %d\n",  pParams->composition.col_num);

#endif //ZZ_BLEND_ALPHA_SUPPORT

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

                ZZPRINTF("Matrix 2002 denoise factor   = %d\n", pParams->denoise.factor);
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

                ZZPRINTF("Matrix 2002 deinterlace algo   = %d\n", pParams->deinterlace.type);
                ZZPRINTF("Matrix 2002 deinterlace bff    = %d\n", pParams->deinterlace.bff_flag);
                ZZPRINTF("Matrix 2002 deinterlace bottom = %d\n", pParams->deinterlace.btm_field_flag);
                ZZPRINTF("Matrix 2002 deinterlace single = %d\n", pParams->deinterlace.one_field_flag);

                sts = render_picture_vp_di(pSelf->ctx->va_dpy, pSelf->ctx->id, &deint_buf_id, &pParams->deinterlace);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("render_picture_vp_di error\n");
                    goto END;
                }

                pSelf->filterBufs[pSelf->numFilterBufs] = deint_buf_id;
                pSelf->numFilterBufs++;
            }
        }
    }

END:
    return sts;
}
