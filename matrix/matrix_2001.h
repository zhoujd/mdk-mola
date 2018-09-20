/* matrix_2001.h --- 
 */

#ifndef INCLUDED_ZZ_MATRIX_2001_H
#define INCLUDED_ZZ_MATRIX_2001_H 1

#include "cmn.h"
#include "matrix_defs.h"

#define MATRIX2001_ID   (2001)

typedef struct _zzMatrix2001ST zzMatrix2001ST;

struct _zzMatrix2001ST
{
    zzMatrixBaseST base;
};


#define GET_MATRIX2001(p) container_of(p, zzMatrix2001ST, base)

zzStatus ZZMatrix2001_Create(zzMatrix2001ST **ppRet);
zzStatus ZZMatrix2001_Release(zzMatrix2001ST *pSelf);

zzStatus ZZMatrix2001_Init(zzMatrix2001ST *pSelf);

zzStatus ZZMatrix2001_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix2001_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_2001_H */

