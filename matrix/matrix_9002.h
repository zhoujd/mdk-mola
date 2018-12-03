/* matrix_9002.h ---
 */

#ifndef INCLUDED_ZZ_MATRIX_9002_H
#define INCLUDED_ZZ_MATRIX_9002_H 1

#include "cmn.h"
#include "matrix_defs.h"
#include "vaapi/frame_writer.h"
#include "vaapi/va_surf_mng.h"

#define MATRIX9002_ID   (9002)

typedef struct _zzMatrix9002ST zzMatrix9002ST;

struct _zzMatrix9002ST
{
    zzMatrixBaseST base;

    zzSurfaceST        src_surf;
    zzFrameWriterST   *pFrameWriter;
};

#define GET_MATRIX9002(p) container_of(p, zzMatrix9002ST, base)

zzStatus ZZMatrix9002_Create(zzMatrix9002ST **ppRet);
zzStatus ZZMatrix9002_Release(zzMatrix9002ST *pSelf);

zzStatus ZZMatrix9002_Init(zzMatrix9002ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf, zz_char *pFilename);

zzStatus ZZMatrix9002_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix9002_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_9002_H */
