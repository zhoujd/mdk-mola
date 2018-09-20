/* matrix_3001.h --- 
 */

#ifndef INCLUDED_ZZ_MATRIX_3001_H
#define INCLUDED_ZZ_MATRIX_3001_H 1

#include "cmn.h"
#include "matrix_defs.h"

#define MATRIX3001_ID   (3001)

typedef struct _zzMatrix3001ST zzMatrix3001ST;

struct _zzMatrix3001ST
{
    zzMatrixBaseST base;
};

#define GET_MATRIX3001(p) container_of(p, zzMatrix3001ST, base)

zzStatus ZZMatrix3001_Create(zzMatrix3001ST **ppRet);
zzStatus ZZMatrix3001_Release(zzMatrix3001ST *pSelf);

zzStatus ZZMatrix3001_Init(zzMatrix3001ST *pSelf);

zzStatus ZZMatrix3001_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix3001_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_3001_H */

