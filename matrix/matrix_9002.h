/* matrix_9002.h ---
 */

#ifndef INCLUDED_ZZ_MATRIX_9002_H
#define INCLUDED_ZZ_MATRIX_9002_H 1

#include "cmn.h"
#include "matrix_defs.h"

#define MATRIX9002_ID   (9002)

typedef struct _zzMatrix9002ST zzMatrix9002ST;

struct _zzMatrix9002ST
{
    zzMatrixBaseST base;
};

#define GET_MATRIX9002(p) container_of(p, zzMatrix9002ST, base)

zzStatus ZZMatrix9002_Create(zzMatrix9002ST **ppRet);
zzStatus ZZMatrix9002_Release(zzMatrix9002ST *pSelf);

zzStatus ZZMatrix9002_Init(zzMatrix9002ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST *pPipeCtrl);


zzStatus ZZMatrix9002_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix9002_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_9002_H */