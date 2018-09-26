/* matrix_1101.h ---
 */

#ifndef INCLUDED_ZZ_MATRIX_1101_H
#define INCLUDED_ZZ_MATRIX_1101_H 1

#include "cmn.h"
#include "matrix_defs.h"
#include "frame_reader.h"
#include "va_surf_mng.h"

#define MATRIX1101_ID (1101)

typedef struct _zzMatrix1101ST zzMatrix1101ST;

struct _zzMatrix1101ST
{
    zzMatrixBaseST    base;

    zzSurfaceST       *pDestSurface;
    zzFrameReaderST   *pFrameReader;  //frame reader
};


#define GET_MATRIX1101(p)  container_of(p, zzMatrix1101ST, base)

zzStatus ZZMatrix1101_Create(zzMatrix1101ST **ppRet);
zzStatus ZZMatrix1101_Release(zzMatrix1101ST *pSelf);

zzStatus ZZMatrix1101_Init(zzMatrix1101ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST *pPipeCtrl);

zzStatus ZZMatrix1101_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix1101_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_1101_H */
