/// va_untils.h

#ifndef VA_UTILS_H
#define VA_UTILS_H 1


#include <va/va.h>
#include <va/va_vpp.h>

#ifndef ANDROID
#if ZZ_LIBVA_USE_X11
#include <va/va_x11.h>
#else //ZZ_LIBVA_USE_X11
#include <va/va_drm.h>
#endif //ZZ_LIBVA_USE_X11
#else //ANDROID
#define Display unsigned int
#endif //ANDROID

#include "cmn.h"

#define ZZ_CTX_DEF_WIDTH   (1920)
#define ZZ_CTX_DEF_HEIGHT  (1200)

#define VAAPI_X_DEFAULT_DISPLAY ":0.0"

typedef struct _zzDisplay zzDisplay;
struct _zzDisplay
{
#ifndef ANDROID
#if ZZ_LIBVA_USE_X11
    Display     *display;
#endif //ZZ_LIBVA_USE_X11
#else
    Display     *display;
#endif //ANDROID

    VADisplay   va_dpy;
};


zzStatus va_to_zz_status(VAStatus va_res);

#ifndef ANDROID
#if ZZ_LIBVA_USE_X11
zzStatus ZZVA_InitVA(Display **display, VADisplay *va_dpy);
zzStatus ZZVA_CloseVA(Display *display,VADisplay va_dpy);
#else //ZZ_LIBVA_USE_X11
zzStatus ZZVA_InitVA(VADisplay *va_dpy);
zzStatus ZZVA_CloseVA(VADisplay va_dpy);
#endif //ZZ_LIBVA_USE_X11
#else //ANDROID
zzStatus ZZVA_InitVA(Display **display, VADisplay *va_dpy);
zzStatus ZZVA_CloseVA(Display *display,VADisplay va_dpy);
#endif //ANDROID

zzStatus ZZVA_GetScreenInfo(zzU16 *pWidth, zzU16 *pHeight);

#endif //VA_UTILS_H
