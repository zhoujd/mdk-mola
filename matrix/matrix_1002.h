/* matrix_1002.h ---
 */

#ifndef INCLUDED_ZZ_MATRIX_1002_H
#define INCLUDED_ZZ_MATRIX_1002_H 1

#include "cmn.h"
#include "matrix_defs.h"
#include "lib/frame_reader.h"
#include "lib/va_surf_mng.h"

#define MATRIX1002_ID    (1002)

typedef struct _zzMatrix1002ST zzMatrix1002ST;
typedef struct _zzMatrix1002InputParamsST zzMatrix1002InputParamsST;

struct _zzMatrix1002ST
{
    zzMatrixBaseST    base;

    zzSurfaceST       dst_surf;
    zzFrameReaderST   *pFrameReader;
    zzU16             frame_idx;
    zzU16             frame_num;

    zzBOOL            demo_flag;
};


#define GET_MATRIX1002(p)  container_of(p, zzMatrix1002ST, base)

zzStatus ZZMatrix1002_Create(zzMatrix1002ST **ppRet);
zzStatus ZZMatrix1002_Release(zzMatrix1002ST *pSelf);

zzStatus ZZMatrix1002_Init(zzMatrix1002ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf, zz_char *pFilename);

zzStatus ZZMatrix1002_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix1002_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_1002_H */
