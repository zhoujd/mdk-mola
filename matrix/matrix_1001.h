/* matrix_1001.h --- 
 */

#ifndef INCLUDED_ZZ_MATRIX_1001_H
#define INCLUDED_ZZ_MATRIX_1001_H 1

#include "cmn.h"
#include "matrix_defs.h"

#define MATRIX1001_ID   (1001)

typedef struct _zzMatrix1001ST zzMatrix1001ST;

struct _zzMatrix1001ST
{
    zzMatrixBaseST base;
};

#define GET_MATRIX1001(p) container_of(p, zzMatrix1001ST, base)

zzStatus ZZMatrix1001_Create(zzMatrix1001ST **ppRet, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix1001_Release(zzMatrix1001ST *pSelf);

zzStatus ZZMatrix1001_Init(zzMatrix1001ST *pSelf);

zzStatus ZZMatrix1001_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix1001_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_1001_H */

