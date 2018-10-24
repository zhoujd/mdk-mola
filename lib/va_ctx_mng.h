/* va_ctx_mng.h
 */

#ifndef INCLUDED_VACTXMNG_H
#define INCLUDED_VACTXMNG_H 1

#include "va_utils.h"

typedef struct _zzVAContextST zzVAContextST;

struct _zzVAContextST
{
#if ZZ_LIBVA_USE_X11
    Display           *display;
#endif //ZZ_LIBVA_USE_X11

    VADisplay         va_dpy;

    zzU16             width;
    zzU16             height;
    zzU32             id;
};

zzStatus ZZVAContext_Create(zzVAContextST *pSelf);
zzStatus ZZVAContext_Release(zzVAContextST *pSelf);

#endif /* INCLUDED_VACTXMNG_H */
