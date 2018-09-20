/* matrix_9001.h --- 
 */

#ifndef INCLUDED_ZZ_MATRIX_9001_H
#define INCLUDED_ZZ_MATRIX_9001_H 1

#include "cmn.h"
#include "matrix_defs.h"

#define MATRIX9001_ID   (9001)

typedef struct _zzMatrix9001ST zzMatrix9001ST;

struct _zzMatrix9001ST
{
    zzMatrixBaseST base;
};

#define GET_MATRIX9001(p) container_of(p, zzMatrix9001ST, base)

zzStatus ZZMatrix9001_Create(zzMatrix9001ST **ppRet);
zzStatus ZZMatrix9001_Release(zzMatrix9001ST *pSelf);

zzStatus ZZMatrix9001_Init(zzMatrix9001ST *pSelf);

zzStatus ZZMatrix9001_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix9001_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_9001_H */

