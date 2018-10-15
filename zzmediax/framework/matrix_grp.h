/* matrix_grp.h
 *
 */

#ifndef INCLUDED_ZZ_MATRIX_GRP_H
#define INCLUDED_ZZ_MATRIX_GRP_H 1

#include "cmn.h"
#include "matrix_defs.h"
#include "va_surf_mng.h"

#define MATRIXGRP_ID    (100)

typedef struct _zzMatrixGRPST zzMatrixGRPST;
typedef struct _zzMatrixGRPInputParamsST zzMatrixGRPInputParamsST;

struct _zzMatrixGRPST
{
    zzMatrixBaseST    base;


    zz_list           matrix_list;
};


#define GET_MATRIXGRP(p)  container_of(p, zzMatrixGRPST, base)

zzStatus ZZMatrixGRP_Create(zzMatrixGRPST **ppRet);
zzStatus ZZMatrixGRP_Release(zzMatrixGRPST *pSelf);

zzStatus ZZMatrixGRP_Init(zzMatrixGRPST *pSelf, zzU16 argc, zz_char **argv,
                          zzPipeCtrlST  *pPipeCtrl);

zzStatus ZZMatrixGRP_RegistMatrix(zzMatrixGRPST *pSelf, zzMatrixBaseST *pMatrix);

zzStatus ZZMatrixGRP_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrixGRP_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_GRP_H */
