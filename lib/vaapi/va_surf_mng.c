/* Last modified Time-stamp: <Jiandong zhou 2007-12-30 16:29:58>
 * @(#)va_surf_mng.c
 */
#include "va_surf_mng.h"

#define GetVaDisplay(p)    (p->context->va_dpy)

static zzStatus ZZSurface_GetGenericFormat(zzU32 format_fourcc, zzU32 *pformat_va);
static zzStatus ZZSurface_FourCC2VaFourCC(zzU32 format_fourcc, int *pformat_va);

zzStatus ZZSurface_CreateSurface(VADisplay display,  int format, int width, int height, VASurfaceID *pSurfaceID, int surface_cnt)
{
    VAStatus va_res  = VA_STATUS_SUCCESS;
    zzStatus sts     = ZZ_ERR_NONE;

    VASurfaceAttrib attrib;

    attrib.type          = VASurfaceAttribPixelFormat;
    attrib.value.type    = VAGenericValueTypeInteger;
    attrib.value.value.i = format;
    attrib.flags         = VA_SURFACE_ATTRIB_SETTABLE;

    va_res = vaCreateSurfaces(display, VA_RT_FORMAT_YUV420, width, height, pSurfaceID,
                              surface_cnt, &attrib, 1);

    sts = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateSurfaces error\n");
        goto END;
    }

END:
    return sts;

}


zzStatus ZZSurface_Create(zzSurfaceST *pSurface)
{
    VAStatus va_res    = VA_STATUS_SUCCESS;
    zzStatus sts       = ZZ_ERR_NONE;
    zzU32    format_va = VA_RT_FORMAT_YUV420;

    VASurfaceAttrib attrib;

    attrib.type       = VASurfaceAttribPixelFormat;
    attrib.value.type = VAGenericValueTypeInteger;
    attrib.flags      = VA_SURFACE_ATTRIB_SETTABLE;

    sts = ZZSurface_FourCC2VaFourCC(pSurface->frameInfo.FourCC, &attrib.value.value.i);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_FourCC2VaFourCC error\n");
        goto END;
    }

    sts = ZZSurface_GetGenericFormat(pSurface->frameInfo.FourCC, &format_va);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_GetGenericFormat error\n");
        goto END;
    }

    va_res = vaCreateSurfaces(GetVaDisplay(pSurface),
                              format_va,
                              pSurface->frameInfo.Width,
                              pSurface->frameInfo.Height,
                              &pSurface->id,
                              1,
                              &attrib,
                              1);

    sts = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaCreateSurfaces error\n");
        goto END;
    }

    ZZDEBUG("%s pSurface->id = %d\n", __FUNCTION__, pSurface->id);

END:
    return sts;

}

zzStatus ZZSurface_Release(zzSurfaceST *pSurface)
{
    VAStatus   va_res  = VA_STATUS_SUCCESS;
    zzStatus sts     = ZZ_ERR_NONE;

    if (VA_INVALID_ID != pSurface->id && !pSurface->ff_lock)
    {
        va_res = vaDestroySurfaces(GetVaDisplay(pSurface), &pSurface->id, 1);
        sts = va_to_zz_status(va_res);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("vaDestroySurfaces error\n");
            goto END;
        }

        pSurface->id = VA_INVALID_ID;
    }

    if (NULL != pSurface->ff_frame)
    {
        sts = ZZSurface_FreeFrame(pSurface);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZSurface_FreeFrame error\n");
            goto END;
        }
    }

END:
    return sts;
}

zzStatus ZZSurface_Lock(zzSurfaceST *pSurface)
{
    VAStatus   va_res  = VA_STATUS_SUCCESS;
    zzStatus sts     = ZZ_ERR_NONE;

    void        *image_data = NULL;
    zzFrameData *ptr = NULL;

    va_res = vaDeriveImage(GetVaDisplay(pSurface), pSurface->id, &pSurface->va_image);
    sts     = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaDeriveImage error\n");
        goto END;
    }

    pSurface->map_flag = TRUE;

    va_res = vaMapBuffer(GetVaDisplay(pSurface), pSurface->va_image.buf, (void *)&image_data);
    sts     = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaMapBuffer error\n");
        goto END;
    }

    ptr = &pSurface->frameData;
    Frame_B(ptr) = Frame_Y(ptr) = image_data;

    switch (pSurface->va_image.format.fourcc)
    {
    case ZZ_FOURCC_400P:
        ptr->Pitch = (zzU16)pSurface->va_image.pitches[0];
        Frame_Y(ptr) = image_data + pSurface->va_image.offsets[0];
        break;
    case ZZ_FOURCC_NV12:
    case ZZ_FOURCC_NV12_LINEAR:
    case ZZ_FOURCC_IYUV:
    case ZZ_FOURCC_IMC3:
    case ZZ_FOURCC_444P:
    case ZZ_FOURCC_411P:
    case ZZ_FOURCC_422H:
    case ZZ_FOURCC_422V:
        ptr->Pitch = (zzU16)pSurface->va_image.pitches[0];
        Frame_Y(ptr) = image_data + pSurface->va_image.offsets[0];
        Frame_U(ptr) = image_data + pSurface->va_image.offsets[1];
        Frame_V(ptr) = image_data + pSurface->va_image.offsets[2];
        break;
    case ZZ_FOURCC_R5G6B5:
    case ZZ_FOURCC_ABGR:
    case ZZ_FOURCC_ARGB:
    case ZZ_FOURCC_XRGB:
        ptr->Pitch = (zzU16)pSurface->va_image.pitches[0];
        Frame_R(ptr) = image_data + pSurface->va_image.offsets[0];
        Frame_G(ptr) = Frame_R(ptr);
        Frame_B(ptr) = Frame_R(ptr);
        break;
    case ZZ_FOURCC_YUY2:
        ptr->Pitch = (zzU16)pSurface->va_image.pitches[0];
        Frame_Y(ptr) = image_data + pSurface->va_image.offsets[0];
        Frame_U(ptr) = image_data + pSurface->va_image.offsets[0];
        Frame_V(ptr) = image_data + pSurface->va_image.offsets[0];
        break;
    case ZZ_FOURCC_YV12:
        ptr->Pitch = (zzU16)pSurface->va_image.pitches[0];
        Frame_Y(ptr) = image_data + pSurface->va_image.offsets[0];
        Frame_U(ptr) = image_data + pSurface->va_image.offsets[2];
        Frame_V(ptr) = image_data + pSurface->va_image.offsets[1];
        break;
    case ZZ_FOURCC_P010:
        ptr->Pitch = (zzU16)pSurface->va_image.pitches[0];
        Frame_Y(ptr) = image_data + pSurface->va_image.offsets[0];
        Frame_U(ptr) = image_data + pSurface->va_image.offsets[1];
        Frame_V(ptr) = image_data + pSurface->va_image.offsets[2];
        break;
    default:
        sts = ZZ_ERR_UNSUPPORTED;
        goto END;
    }

END:
    return sts;

}

zzStatus ZZSurface_UnLock(zzSurfaceST *pSurface)
{
    zzStatus sts     = ZZ_ERR_NONE;
    VAStatus   va_res  = VA_STATUS_SUCCESS;

    if (pSurface->map_flag == FALSE)
    {
        ZZPRINTF("pSurface->map_flag == FALSE\n");
        goto END;
    }

    va_res = vaUnmapBuffer(GetVaDisplay(pSurface), pSurface->va_image.buf);
    sts     = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaUnmapBuffer error\n");
        goto END;
    }

    va_res = vaDestroyImage(GetVaDisplay(pSurface), pSurface->va_image.image_id);
    sts     = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaDestroyImage error\n");
        goto END;
    }

END:

    return sts;

}


zzStatus ZZSurface_GetGenericFormat(zzU32 format_fourcc, zzU32 *pformat_va)
{
    zzStatus sts     = ZZ_ERR_NONE;

    switch (format_fourcc)
    {
    case ZZ_FOURCC_400P:
        *pformat_va = VA_RT_FORMAT_YUV400;
        break;
    case ZZ_FOURCC_411P:
        *pformat_va = VA_RT_FORMAT_YUV411;
        break;
    case ZZ_FOURCC_YV12:
    case ZZ_FOURCC_IYUV:
    case ZZ_FOURCC_IMC3:
    case ZZ_FOURCC_NV12:
        *pformat_va = VA_RT_FORMAT_YUV420;
        break;
    case ZZ_FOURCC_ABGR:
    case ZZ_FOURCC_ARGB:
    case ZZ_FOURCC_XRGB:
        *pformat_va = VA_RT_FORMAT_RGB32;
        break;
    case ZZ_FOURCC_R5G6B5:
        *pformat_va = ZZ_FOURCC_R5G6B5;  //will be replace by VA_RT_FORMAT_RGB16 when libva ready
        break;
    case ZZ_FOURCC_YUY2:
    case ZZ_FOURCC_422H:
    case ZZ_FOURCC_422V:
        *pformat_va = VA_RT_FORMAT_YUV422;
        break;
    case ZZ_FOURCC_444P:
        *pformat_va = VA_RT_FORMAT_YUV444;
        break;
    case ZZ_FOURCC_NV12_LINEAR:
        *pformat_va = ZZ_FOURCC_NV12_LINEAR;
        break;
    case ZZ_FOURCC_P010:
        *pformat_va = VA_RT_FORMAT_YUV420_10;
        break;
    case ZZ_FOURCC_A2RGB10:
        *pformat_va = VA_RT_FORMAT_RGB32_10;
        break;
    default:
        sts = ZZ_ERR_UNSUPPORTED;
        break;
    }

    return sts;

}


zzStatus ZZSurface_FourCC2VaFourCC(zzU32 format_fourcc, int *pformat_va)
{
    zzStatus sts     = ZZ_ERR_NONE;

    switch (format_fourcc)
    {
    case ZZ_FOURCC_YV12:
        *pformat_va = VA_FOURCC_YV12;
        break;
    case ZZ_FOURCC_IMC3:
        *pformat_va = VA_FOURCC_IMC3;
        break;
#if 0 //zhoujd
    case ZZ_FOURCC_400P:
        *pformat_va = VA_FOURCC_400P;
        break;
#endif //zhoujd
    case ZZ_FOURCC_411P:
        *pformat_va = VA_FOURCC_411P;
        break;
    case ZZ_FOURCC_444P:
        *pformat_va = VA_FOURCC_444P;
        break;
    case ZZ_FOURCC_IYUV:
        *pformat_va = VA_FOURCC_IYUV;
        break;
    case ZZ_FOURCC_422H:
        *pformat_va = VA_FOURCC_422H;
        break;
    case ZZ_FOURCC_422V:
        *pformat_va = VA_FOURCC_422V;
        break;
    case ZZ_FOURCC_NV12:
        *pformat_va = VA_FOURCC_NV12;
        break;
    case ZZ_FOURCC_NV12_LINEAR:
        *pformat_va = ZZ_FOURCC_NV12_LINEAR;
        break;
    case ZZ_FOURCC_YUY2:
        *pformat_va = VA_FOURCC_YUY2;
        break;
    case ZZ_FOURCC_ABGR:
        *pformat_va = ZZ_FOURCC_ABGR;  //will be replace by VA_FOURCC_ABGR when libva ready
        break;
    case ZZ_FOURCC_R5G6B5:
        *pformat_va = ZZ_FOURCC_R5G6B5;  //will be replace by VA_FOURCC_R5G6B5 when libva ready
        break;
    case ZZ_FOURCC_ARGB:
        *pformat_va = VA_FOURCC_ARGB;
        break;
    case ZZ_FOURCC_XRGB:
        *pformat_va = VA_FOURCC_XRGB;
        break;
    case ZZ_FOURCC_P010:
        *pformat_va = VA_FOURCC_P010;
        break;
    case ZZ_FOURCC_A2RGB10:
        *pformat_va = VA_FOURCC_RGBA;
        break;
    default:
        sts = ZZ_ERR_UNSUPPORTED;
        break;
    }

    return sts;

}

zzStatus ZZSurface_GetNextInputFrame(zzSurfaceST *pSurface, zzFrameReaderST *pFrameReader)
{
    zzStatus  sts       = ZZ_ERR_NONE;
    zzStatus  readSts   = ZZ_ERR_NONE;
    zzBOOL    lock_flag = TRUE;

    CHECK_POINTER(pSurface, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pFrameReader, ZZ_ERR_NOT_INITIALIZED);

    sts = ZZSurface_Lock(pSurface);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_Lock error\n");
        goto END;
    }

    lock_flag = TRUE;


    readSts = ZZFrameReader_GetNextInputFrame(pFrameReader,
                                                &pSurface->frameData,
                                                &pSurface->frameInfo);
    if ((readSts != ZZ_ERR_NONE) && (readSts != ZZ_ERR_EOF_STREAM))
    {
        ZZPRINTF("ZZFrameReader_GetNextInputFrame error\n");
        sts = readSts;
        goto END;
    }

END:
    if (lock_flag == TRUE)
    {
        sts = ZZSurface_UnLock(pSurface);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZSurface_UnLock error\n");
        }
    }

    if (readSts == ZZ_ERR_EOF_STREAM)
    {
        sts = readSts;
    }

    return sts;
}

zzStatus ZZSurface_WriteOutputFrame(zzSurfaceST *pSurface, zzFrameWriterST *pFrameWriter)
{
    zzStatus  sts       = ZZ_ERR_NONE;
    zzBOOL    lock_flag = FALSE;

    sts = ZZSurface_Lock(pSurface);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_Lock error\n");
        goto END;
    }

    lock_flag = TRUE;

    sts = ZZFrameWriter_WriteFrame(pFrameWriter, &pSurface->frameData, &pSurface->frameInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZFrameWriter_WriteFrame error\n");
        goto END;
    }

END:
    if (lock_flag == TRUE)
    {
        sts = ZZSurface_UnLock(pSurface);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZSurface_UnLock error\n");
        }
    }

    return sts;

}

zzStatus ZZSurface_AllocFrame(zzSurfaceST *pSurface)
{
    zzStatus  sts       = ZZ_ERR_NONE;

    if (!(pSurface->ff_frame = av_frame_alloc()))
        return AVERROR(ENOMEM);

    return sts;
}

zzStatus ZZSurface_FreeFrame(zzSurfaceST *pSurface)
{
    zzStatus  sts       = ZZ_ERR_NONE;

    av_frame_free(&pSurface->ff_frame);

    return sts;
}

zzStatus ZZSurface_LockFrame(zzSurfaceST *pSurface)
{
    zzStatus  sts  = ZZ_ERR_NONE;
    VAStatus  vas  = VA_STATUS_SUCCESS;


    pSurface->id = (VASurfaceID)(uintptr_t)pSurface->ff_frame->data[3];
    pSurface->ff_lock = TRUE;

#if 0 //zhoujd
    vas = vaSyncSurface(GetVaDisplay(pSurface), pSurface->id);
#endif //zhoujd

    if (vas != VA_STATUS_SUCCESS) {
        ZZDEBUG("error vaSyncSurface surface id = %d\n", pSurface->id);
        sts = AVERROR(EIO);
        return sts;
    }


    ZZDEBUG("FF surface id = %d\n", pSurface->id);

    return sts;
}
