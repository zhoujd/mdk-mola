/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)va_render_utils.c
 */

#include "va_render_utils.h"

static zzStatus  render_check_factor(float value, float max_value, float min_value, float step);

zzStatus  render_check_factor(float value, float max_value, float min_value, float step)
{
    zzStatus sts = ZZ_ERR_NONE;

    ZZPRINTF("Render: factor=%f, max_value=%f, min_value=%f, step=%f\n", value, max_value, min_value, step);

    return sts;

}

zzStatus  render_picture_vp_frc(VADisplay display, VAContextID ctx_id, VABufferID *frc_buf_id, zzU32 input_fps, zzU32 output_fps, zzU32 num_frameIDs, VASurfaceID *pID)
{
    VAStatus   ret = VA_STATUS_SUCCESS;
    zzStatus sts = ZZ_ERR_NONE;

    VAProcFilterParameterBufferFrameRateConversion frc_param;

    //init
    ZERO_MEMORY(frc_param);

#if ZZ_FRC_SUPPORT //zhoujd
    frc_param.type              = VAProcFilterFrameRateConversion;
    frc_param.input_fps         = input_fps;
    frc_param.output_fps        = output_fps;
    frc_param.num_output_frames = num_frameIDs;
    frc_param.output_frames     = pID;

    ret = vaCreateBuffer(display,
                         ctx_id,
                         VAProcFilterParameterBufferType,
                         sizeof(frc_param),
                         1,
                         &frc_param,
                         frc_buf_id);
#endif //zhoujd
    sts     = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateBuffer error\n");
        goto END;
    }

END:

    return sts;
}

zzStatus render_picture_vp_ief(VADisplay display, VAContextID ctx_id, VABufferID *sharp_buf_id, zzU16 ief_factor)
{
    VAStatus   ret = VA_STATUS_SUCCESS;
    zzStatus sts = ZZ_ERR_NONE;

    zzU32                     caps_num;
    VAProcFilterCap             sharp_caps;
    VAProcFilterParameterBuffer sharp_param;

    //init
    ZERO_MEMORY(sharp_caps);
    ZERO_MEMORY(sharp_param);
    caps_num = 1;

#if ZZ_QUERY_CAPS_SUPPORT
    //query capbilities value for sharpness
    ret = vaQueryVideoProcFilterCaps(display,
                                     ctx_id,
                                     VAProcFilterSharpening,
                                     &sharp_caps,
                                     &caps_num);

    sts = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaQueryVideoProcFilterCaps error\n");
        goto END;
    }

    sts = render_check_factor(ief_factor, sharp_caps.range.max_value, sharp_caps.range.min_value, sharp_caps.range.step);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("render_check_factor error\n");
        goto END;
    }
#endif //ZZ_QUERY_CAPS_SUPPORT

    //TODO: using sharp_caps check input value.
    sharp_param.type  = VAProcFilterSharpening;
    sharp_param.value = ief_factor;
    //create buffer
    ret = vaCreateBuffer(display,
                         ctx_id,
                         VAProcFilterParameterBufferType,
                         sizeof(sharp_param),
                         1,
                         &sharp_param,
                         sharp_buf_id);
    sts = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateBuffer error\n");
        goto END;
    }


END:
    return sts;
}

zzStatus render_picture_vp_di(VADisplay display, VAContextID ctx_id, VABufferID *deint_buf_id, zzDeinterlaceParamST *pParam)
{
    VAStatus   ret      = VA_STATUS_SUCCESS;
    zzStatus sts      = ZZ_ERR_NONE;
    int        i        = 0;
    zzBOOL   bSupport = FALSE;

    zzU32                                  caps_num = 0;
    VAProcFilterCapDeinterlacing             deint_caps[VAProcDeinterlacingCount];
    VAProcFilterParameterBufferDeinterlacing deint_param;

    //init
    ZERO_MEMORY(deint_caps);
    ZERO_MEMORY(deint_param);
    caps_num = sizeof(deint_caps) / sizeof(deint_caps[0]);

#if ZZ_QUERY_CAPS_SUPPORT
    ret = vaQueryVideoProcFilterCaps(display,
                                     ctx_id,
                                     VAProcFilterDeinterlacing,
                                     deint_caps,
                                     &caps_num);
    sts     = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaQueryVideoProcFilterCaps error\n");
        goto END;
    }

    for (i = 0; i < caps_num; i++)
    {
        if (deint_caps[i].type == pParam->type)
        {
            ZZPRINTF("render: driver support di type = %d\n", pParam->type);
            bSupport = TRUE;
            break;
        }
    }

    if (bSupport == FALSE)
    {
        ZZPRINTF("render: driver unsupport di type = %d\n", pParam->type);
        return ZZ_ERR_UNSUPPORTED;
    }
#endif //ZZ_QUERY_CAPS_SUPPORT

    //TODO: using deint_caps check input value.
    deint_param.type  = VAProcFilterDeinterlacing;
    deint_param.algorithm = pParam->type;

#if ZZ_UPDATE_DI_SUPPORT
    if (pParam->bff_flag)
    {
        deint_param.flags |= VA_DEINTERLACING_BOTTOM_FIELD_FIRST;
    }
    if (pParam->btm_field_flag)
    {
        deint_param.flags |= VA_DEINTERLACING_BOTTOM_FIELD;
    }
    if (pParam->one_field_flag)
    {
        deint_param.flags |= VA_DEINTERLACING_ONE_FIELD;
    }
#endif //ZZ_UPDATE_DI_SUPPORT

    ret = vaCreateBuffer(display,
                         ctx_id,
                         VAProcFilterParameterBufferType,
                         sizeof(deint_param),
                         1,
                         &deint_param,
                         deint_buf_id);

    sts     = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateBuffer error\n");
        goto END;
    }


END:
    return sts;
}


zzStatus render_picture_vp_hsbc(VADisplay display, VAContextID ctx_id, VABufferID *hsbc_buf_id, zzF64 hue, zzF64 saturation, zzF64 brightness, zzF64 contrast)
{
    VAStatus   ret = VA_STATUS_SUCCESS;
    zzStatus sts = ZZ_ERR_NONE;
    int        i   = 0;

    enum { kHue, kSaturation, kBrightness, kContrast, kCount };
    VAProcFilterParameterBufferColorBalance hsbc_param[kCount];

    zzU32                     caps_num;
    VAProcFilterCapColorBalance hsbc_caps[kCount];

    //init
    ZERO_MEMORY(hsbc_param);
    ZERO_MEMORY(hsbc_caps);
    caps_num = kCount;

#if ZZ_QUERY_CAPS_SUPPORT
    ret = vaQueryVideoProcFilterCaps(display,
                                     ctx_id,
                                     VAProcFilterColorBalance,
                                     &hsbc_caps,
                                     &caps_num);
    sts     = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaQueryVideoProcFilterCaps error\n");
        goto END;
    }

    for (i = 0; i < kCount; i++)
    {
        zzF64                     value;
        switch (hsbc_caps[i].type)
        {
        case VAProcColorBalanceHue:
            ZZPRINTF("Render-hsbc-hue\n:");
            value = hue;
            break;
        case VAProcColorBalanceSaturation:
            ZZPRINTF("Render-hsbc-saturation\n:");
            value = saturation;
            break;
        case VAProcColorBalanceBrightness:
            ZZPRINTF("Render-hsbc-brightness\n:");
            value = brightness;
            break;
        case VAProcColorBalanceContrast:
            ZZPRINTF("Render-hsbc-contrast\n:");
            value = contrast;
            break;
        default:
            break;
        }

        sts = render_check_factor(value, hsbc_caps[i].range.max_value, hsbc_caps[i].range.min_value, hsbc_caps[i].range.step);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("render_check_factor error\n");
            goto END;
        }

    }
#endif //ZZ_QUERY_CAPS_SUPPORT

    hsbc_param[kHue].type   = VAProcFilterColorBalance;
    hsbc_param[kHue].attrib = VAProcColorBalanceHue;
    hsbc_param[kHue].value  = hue;

    hsbc_param[kSaturation].type   = VAProcFilterColorBalance;
    hsbc_param[kSaturation].attrib = VAProcColorBalanceSaturation;
    hsbc_param[kSaturation].value  = saturation;

    hsbc_param[kBrightness].type   = VAProcFilterColorBalance;
    hsbc_param[kBrightness].attrib = VAProcColorBalanceBrightness;
    hsbc_param[kBrightness].value  = brightness;

    hsbc_param[kContrast].type   = VAProcFilterColorBalance;
    hsbc_param[kContrast].attrib = VAProcColorBalanceContrast;
    hsbc_param[kContrast].value  = contrast;

    ret = vaCreateBuffer(display,
                         ctx_id,
                         VAProcFilterParameterBufferType,
                         sizeof(hsbc_param),
                         kCount,
                         &hsbc_param,
                         hsbc_buf_id);

    sts     = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateBuffer error\n");
        goto END;
    }


END:
    return sts;
}

zzStatus render_picture_vp_dn(VADisplay display, VAContextID ctx_id, VABufferID *noise_buf_id, zzU64 denoise)
{
    VAStatus   ret = VA_STATUS_SUCCESS;
    zzStatus   sts = ZZ_ERR_NONE;

    zzU32                       caps_num;
    VAProcFilterCap             noise_caps;
    VAProcFilterParameterBuffer noise_param;

    //init
    ZERO_MEMORY(noise_caps);
    ZERO_MEMORY(noise_param);
    caps_num = 1;

#if ZZ_QUERY_CAPS_SUPPORT
    ret = vaQueryVideoProcFilterCaps(display,
                                     ctx_id,
                                     VAProcFilterNoiseReduction,
                                     &noise_caps,
                                     &caps_num);
    sts     = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaQueryVideoProcFilterCaps error\n");
        goto END;
    }

    sts = render_check_factor(denoise, noise_caps.range.max_value, noise_caps.range.min_value, noise_caps.range.step);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("render_check_factor error\n");
        goto END;
    }
#endif //ZZ_QUERY_CAPS_SUPPORT

    noise_param.type = VAProcFilterNoiseReduction;
    noise_param.value = denoise;

    ret = vaCreateBuffer(display,
                         ctx_id,
                         VAProcFilterParameterBufferType,
                         sizeof(noise_param),
                         1,
                         &noise_param,
                         noise_buf_id);

    sts     = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateBuffer error\n");
        goto END;
    }


END:
    return sts;
}

zzStatus render_picture_vp_check_pipeline(VADisplay display, VAContextID ctx_id)
{
    VAStatus   ret = VA_STATUS_SUCCESS;
    zzStatus sts = ZZ_ERR_NONE;

    VAProcPipelineCaps pipeline_cap;

    //init
    ZERO_MEMORY(pipeline_cap);

#if 0 //zhoujd may be check in further
    //query caps for pipeline
    ret = vaQueryVideoProcPipelineCaps(display,
                                       ctx_id,
                                       NULL,
                                       0,
                                       &pipeline_cap);

    sts     = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaQueryVideoProcPipelineCaps error\n");
        goto END;
    }

END:
#endif //zhoujd

    return sts;
}

zzStatus render_picture_vp_primary(VADisplay display, VAContextID ctx_id, zzPipelineBufferST *pParams, VABufferID *primary_buf_id)
{
    VAStatus   ret = VA_STATUS_SUCCESS;
    zzStatus   sts = ZZ_ERR_NONE;
    int        i   = 0;

    VAProcPipelineParameterBuffer primary_param;

    //init
    ZERO_MEMORY(primary_param);

    //region setting
    primary_param.surface_region = pParams->src;
    primary_param.output_region  = pParams->dst;

    //src surface
    primary_param.surface = (VASurfaceID)pParams->surface;

    //color_standard setting
    primary_param.surface_color_standard = pParams->surface_color_standard;
    primary_param.output_color_standard  = pParams->output_color_standard;

    //colorfill
    primary_param.output_background_color = pParams->color_fill;

    //filter flag
    primary_param.filter_flags = pParams->filter_flags;

    //filters
    primary_param.filters     = pParams->filters;
    primary_param.num_filters = pParams->filter_num;

    //reference frame setting
    primary_param.forward_references      = pParams->forward_references;
    primary_param.num_forward_references  = pParams->num_forward_references;
    primary_param.backward_references     = pParams->backward_references;
    primary_param.num_backward_references = pParams->num_backward_references;

    ret = vaCreateBuffer(display,
                         ctx_id,
                         VAProcPipelineParameterBufferType,
                         sizeof(VAProcPipelineParameterBuffer),
                         1,
                         &primary_param,
                         primary_buf_id);

    sts = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateBuffer error\n");
        goto END;
    }


END:
    return sts;
}

zzStatus render_picture_vp_subpicture(VADisplay  display, VAContextID ctx_id, zzPipelineBufferST *pParams, VABufferID *subpicture_buf_id)
{
    VAStatus ret   = VA_STATUS_SUCCESS;
    zzStatus sts = ZZ_ERR_NONE;

    VAProcPipelineParameterBuffer subpicture_param;

    //init
    ZERO_MEMORY(subpicture_param);

    //src rect
    subpicture_param.surface_region = pParams->src;

    //dst rect for scaling
    subpicture_param.output_region = pParams->dst;
    //render target
    subpicture_param.surface = (VASurfaceID)pParams->surface;
    //csc
    subpicture_param.surface_color_standard = pParams->surface_color_standard;
    subpicture_param.output_color_standard = pParams->output_color_standard;
    //colorfill
    subpicture_param.output_background_color = pParams->color_fill;

    //filter flag
    subpicture_param.pipeline_flags |= VA_PROC_PIPELINE_FAST;
    subpicture_param.filter_flags |= VA_FILTER_SCALING_FAST;
    //filters
    subpicture_param.filters = pParams->filters;
    subpicture_param.num_filters = pParams->filter_num;

    //subpicture_param.forward_references = 0;
    //subpicture_param.num_forward_references = 0;
    //subpicture_param.backward_references = 0;
    //subpicture_param.num_backward_references = 0;

    ret = vaCreateBuffer(display,
                         ctx_id,
                         VAProcPipelineParameterBufferType,
                         sizeof(VAProcPipelineParameterBuffer),
                         1,
                         &subpicture_param,
                         subpicture_buf_id);

    sts = va_to_zz_status(ret);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateBuffer error\n");
        goto END;
    }

END:
    return sts;
}
