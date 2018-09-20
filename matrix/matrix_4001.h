/* matrix_4001.h --- 
 */

#ifndef INCLUDED_ZZ_MATRIX_4001_H
#define INCLUDED_ZZ_MATRIX_4001_H 1

#include "cmn.h"
#include "matrix_defs.h"

#define MATRIX4001_ID   (4001)

typedef struct _zzMatrix4001ST zzMatrix4001ST;

struct _zzMatrix4001ST
{
    zzMatrixBaseST base;
};

#define GET_MATRIX4001(p) container_of(p, zzMatrix4001ST, base)

zzStatus ZZMatrix4001_Create(zzMatrix4001ST **ppRet);
zzStatus ZZMatrix4001_Release(zzMatrix4001ST *pSelf);

zzStatus ZZMatrix4001_Init(zzMatrix4001ST *pSelf);

zzStatus ZZMatrix4001_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix4001_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_4001_H */

