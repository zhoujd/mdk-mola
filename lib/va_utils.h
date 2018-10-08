/// va_untils.h

#ifndef VA_UTILS_H
#define VA_UTILS_H 1


#include <va/va.h>
#include <va/va_vpp.h>


#if ZZ_LIBVA_USE_X11
#include <va/va_x11.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#else
#define Display unsigned int
#include <va/va_drm.h>
#endif //ZZ_LIBVA_USE_X11

#include "cmn.h"

#define ZZ_CTX_DEF_WIDTH   (1920)
#define ZZ_CTX_DEF_HEIGHT  (1200)

#define VAAPI_X_DEFAULT_DISPLAY ":0.0"

typedef struct _zzDisplay zzDisplay;
struct _zzDisplay
{
    Display     *display;
    VADisplay   va_dpy;
};


zzStatus va_to_zz_status(VAStatus va_res);
zzStatus ZZVA_InitVA(Display **display, VADisplay *va_dpy);
zzStatus ZZVA_CloseVA(Display *display,VADisplay va_dpy);
zzStatus ZZVA_GetScreenInfo(zzU16 *pWidth, zzU16 *pHeight);

#endif //VA_UTILS_H
