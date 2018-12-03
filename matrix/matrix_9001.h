/* matrix_9001.h ---
 */

#ifndef INCLUDED_ZZ_MATRIX_9001_H
#define INCLUDED_ZZ_MATRIX_9001_H 1

#include <va/va_x11.h>
#include <X11/Xlib.h>

#include "cmn.h"
#include "matrix_defs.h"
#include "vaapi/va_surf_mng.h"

#define MATRIX9001_ID   (9001)

typedef struct _zzMatrix9001ST zzMatrix9001ST;

struct _zzMatrix9001ST
{
    zzMatrixBaseST     base;

    zzSurfaceST        src_surf;

    Display           *mDisplay;
    zzI32              mScreen;
    Window             mRootWin;
    Window             mShowWin;
    GC                 mGC;
    XGCValues          mGS;

    zzU32              srcX;
    zzU32              srcY;
    zzU32              srcW;
    zzU32              srcH;

    zzU32              dstX;
    zzU32              dstY;
    zzU32              dstW;
    zzU32              dstH;

    zzU32              flags;
    zzF32              fps;
};

#define GET_MATRIX9001(p) container_of(p, zzMatrix9001ST, base)

zzStatus ZZMatrix9001_Create(zzMatrix9001ST **ppRet);
zzStatus ZZMatrix9001_Release(zzMatrix9001ST *pSelf);

zzStatus ZZMatrix9001_Init(zzMatrix9001ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf);

zzStatus ZZMatrix9001_ProcNextFrame(zzMatrix9001ST *pSelf);

zzStatus ZZMatrix9001_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix9001_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_9001_H */
