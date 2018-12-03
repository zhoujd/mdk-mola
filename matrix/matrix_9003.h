/* matrix_9003.h ---
 */

#ifndef INCLUDED_ZZ_MATRIX_9003_H
#define INCLUDED_ZZ_MATRIX_9003_H 1

#include "cmn.h"
#include "matrix_defs.h"
#include "vaapi/frame_writer.h"
#include "vaapi/va_surf_mng.h"

#define MATRIX9003_ID   (9003)

typedef struct _zzMatrix9003ST zzMatrix9003ST;

struct _zzMatrix9003ST
{
    zzMatrixBaseST base;

    zzSurfaceST        src_surf;
    zzFrameWriterST   *pFrameWriter;
};

#define GET_MATRIX9003(p) container_of(p, zzMatrix9003ST, base)

zzStatus ZZMatrix9003_Create(zzMatrix9003ST **ppRet);
zzStatus ZZMatrix9003_Release(zzMatrix9003ST *pSelf);

zzStatus ZZMatrix9003_Init(zzMatrix9003ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf, zz_char *pFilename);

zzStatus ZZMatrix9003_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix9003_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_9003_H */
