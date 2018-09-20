/// va_utils.c

#include "va_utils.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <math.h>


zzStatus va_to_zz_status(VAStatus va_res)
{
    zzStatus zzRes = ZZ_ERR_NONE;

    switch (va_res)
    {
    case VA_STATUS_SUCCESS:
        zzRes = ZZ_ERR_NONE;
        break;
    case VA_STATUS_ERROR_ALLOCATION_FAILED:
        zzRes = ZZ_ERR_MEMORY_ALLOC;
        break;
    case VA_STATUS_ERROR_ATTR_NOT_SUPPORTED:
    case VA_STATUS_ERROR_UNSUPPORTED_PROFILE:
    case VA_STATUS_ERROR_UNSUPPORTED_ENTRYPOINT:
    case VA_STATUS_ERROR_UNSUPPORTED_RT_FORMAT:
    case VA_STATUS_ERROR_UNSUPPORTED_BUFFERTYPE:
    case VA_STATUS_ERROR_FLAG_NOT_SUPPORTED:
    case VA_STATUS_ERROR_RESOLUTION_NOT_SUPPORTED:
        zzRes = ZZ_ERR_UNSUPPORTED;
        break;
    case VA_STATUS_ERROR_INVALID_DISPLAY:
    case VA_STATUS_ERROR_INVALID_CONFIG:
    case VA_STATUS_ERROR_INVALID_CONTEXT:
    case VA_STATUS_ERROR_INVALID_SURFACE:
    case VA_STATUS_ERROR_INVALID_BUFFER:
    case VA_STATUS_ERROR_INVALID_IMAGE:
    case VA_STATUS_ERROR_INVALID_SUBPICTURE:
        zzRes = ZZ_ERR_NOT_INITIALIZED;
        break;
    case VA_STATUS_ERROR_INVALID_PARAMETER:
        zzRes = ZZ_ERR_INVALID_VIDEO_PARAM;
    case VA_STATUS_ERROR_UNIMPLEMENTED:
        ZZPRINTF("VPPG status: VA_STATUS_ERROR_UNIMPLEMENTED\n");
        zzRes = ZZ_ERR_UNSUPPORTED;
    default:
        zzRes = ZZ_ERR_UNKNOWN;
        break;
    }
    return zzRes;
}

#ifndef ANDROID
#if ZZ_LIBVA_USE_X11
zzStatus ZZVA_InitVA_X11(Display **display, VADisplay *va_dpy)
{
    VAStatus   va_res  = VA_STATUS_SUCCESS;
    zzStatus sts     = ZZ_ERR_NONE;
    
    int       major_version = 0;
    int       minor_version = 0;

    char*     currentDisplay = NULL;
    currentDisplay = getenv("DISPLAY");
    if (currentDisplay)
    {
        *display = XOpenDisplay(currentDisplay);
    }
    else
    {
        *display = XOpenDisplay(VAAPI_X_DEFAULT_DISPLAY);
    }

    if (NULL == *display)
    {
        sts = ZZ_ERR_NOT_INITIALIZED;
        ZZPRINTF("XOpenDisplay error!\n");
        goto END;
    }
    
    *va_dpy = vaGetDisplay(*display);
    va_res  = vaInitialize(*va_dpy, &major_version, &minor_version);
    sts     = va_to_zz_status(va_res);
    if (ZZ_ERR_NONE != sts)
    {
        ZZPRINTF("vaInitialize error!\n");
        goto END;
    }
    
END:
    return sts;
    
}    
#else //ZZ_LIBVA_USE_X11
zzStatus ZZVA_InitVA_DRM(Display **display, VADisplay *va_dpy)
{
    VAStatus   va_res  = VA_STATUS_SUCCESS;
    zzStatus sts     = ZZ_ERR_NONE;
    
    int       major_version = 0;
    int       minor_version = 0;

    static int drm_fd = -1;
    drm_fd = open("/dev/dri/card0", O_RDWR);
    if (drm_fd < 0)
    {
        ZZPRINTF("error: can't open DRM connection!\n");
        sts = ZZ_ERR_UNKNOWN;
        goto END;
    }
    
    *va_dpy = vaGetDisplayDRM(drm_fd);
    if (*va_dpy == NULL)
    {
        ZZPRINTF("error: va_dpy == NULL!\n");
        sts = ZZ_ERR_UNKNOWN;
        goto END;
    }
    
    va_res  = vaInitialize(*va_dpy, &major_version, &minor_version);
    sts     = va_to_zz_status(va_res);
    if (ZZ_ERR_NONE != sts)
    {
        ZZPRINTF("vaInitialize error!\n");
        goto END;
    }
    
END:
    return sts;
}
#endif //ZZ_LIBVA_USE_X11

#else
zzStatus ZZVA_InitVA_Android(Display **display, VADisplay *va_dpy)
{
    VAStatus   va_res  = VA_STATUS_SUCCESS;
    zzStatus sts     = ZZ_ERR_NONE;
    
    int       major_version = 0;
    int       minor_version = 0;

    *display = (Display*)AllocAndZeroMem(sizeof(Display));
    **(display) = 0x18c34078;

    if (NULL == *display)
    {
        sts = ZZ_ERR_NOT_INITIALIZED;
        ZZPRINTF("NULL == *display error\n");
        goto END;
    }
    
    *va_dpy = vaGetDisplay(*display);
    va_res  = vaInitialize(*va_dpy, &major_version, &minor_version);
    sts     = va_to_zz_status(va_res);
    if (ZZ_ERR_NONE != sts)
    {
        ZZPRINTF("vaInitialize error!\n");
        goto END;
    }
    
END:
    return sts;
}
#endif //ANDROID

zzStatus ZZVA_InitVA(Display **display, VADisplay *va_dpy)
{

#ifdef ANDROID
    return ZZVA_InitVA_Android(display, va_dpy);
#else //ANDROID
    
#if ZZ_LIBVA_USE_X11
    return ZZVA_InitVA_X11(display, va_dpy);
#else
    return ZZVA_InitVA_DRM(display, va_dpy);
#endif //ZZ_LIBVA_USE_X11

#endif //ANDROID
    
}// zzStatus InitVA()

zzStatus ZZVA_CloseVA(Display *display,VADisplay va_dpy)
{
    zzStatus sts     = ZZ_ERR_NONE;
    
    vaTerminate(va_dpy);
    
    if (NULL != display)
    {
#ifndef ANDROID

#if ZZ_LIBVA_USE_X11
        XCloseDisplay(display);
        display = NULL;
#endif //ZZ_LIVA_USE_X11
        
#else
        free(display);
#endif
    }

    return sts;
} //void CloseVA()

zzStatus ZZVA_GetScreenInfo(zzU16 *pWidth, zzU16 *pHeight)
{
#ifndef ANDROID

#if ZZ_LIBVA_USE_X11    
    char         *display_name;
    Display      *display;

    int           screen_num;
    unsigned int  display_width;
    unsigned int  display_height;

    display_name   = getenv("DISPLAY");
    display        = XOpenDisplay(display_name);
    screen_num     = DefaultScreen(display);
    display_width  = DisplayWidth(display, screen_num);
    display_height = DisplayHeight(display, screen_num);
    
    CHECK_POINTER(pWidth, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pHeight, ZZ_ERR_NOT_INITIALIZED);
    
    *pWidth  = display_width;
    *pHeight = display_height;
#else    
    *pWidth  = ZZ_CTX_DEF_WIDTH;
    *pHeight = ZZ_CTX_DEF_HEIGHT;
#endif //VPPG_LIBVA_USE_X11    
    
#else
    *pWidth  = ZZ_CTX_DEF_WIDTH;
    *pHeight = ZZ_CTX_DEF_HEIGHT;
#endif
    
    return ZZ_ERR_NONE;
}
