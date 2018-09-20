/* task_defs.h --- 
 */

#ifndef INCLUDED_ZZ_TASK_DEFS_H
#define INCLUDED_ZZ_TASK_DEFS_H 1


#include "cmn.h"
#include "matrix_defs.h"

#define MAX_TASK_FLOW_NUM   (100)

typedef struct _zzTaskBaseST zzTaskBaseST;

typedef zzStatus (*zzTaskInitFn)(zzTaskBaseST *pSelf, zzU16 argc, zz_char **argv);
typedef zzStatus (*zzTaskExecInitFn)(zzTaskBaseST *pSelf);
typedef zzStatus (*zzTaskReleaseFn)(zzTaskBaseST *pSelf);
typedef zzStatus (*zzTaskHelpFn)(zzTaskBaseST *pSelf, zzU16 argc, zz_char **argv);

struct _zzTaskBaseST
{
    zzU16                   task_id;

    zz_char                 *desc_info;                 //help doc string

    zzTaskInitFn            pfnZZTaskInit;
    zzTaskExecInitFn        pfnZZTaskExecInit;
    zzTaskReleaseFn         pfnZZTaskRelease;
    zzTaskHelpFn            pfnZZTaskHelp;

    zz_list                 task_list;

    //matrix list
    zz_list                 matrix_head;
};

zzStatus ZZTaskBase_DefaultFP(zzTaskBaseST *pSelf);
zzStatus ZZTaskBase_RegisterFP(zzTaskBaseST         *pSelf,
                               zzTaskInitFn         pfnZZTaskInit,
                               zzTaskExecInitFn     pfnZZTaskExecInit,
                               zzTaskReleaseFn      pfnZZTaskRelease,
                               zzTaskHelpFn         pfnZZTaskHelp);

zzStatus ZZTaskBase_AttachMatrix(zzTaskBaseST *pSelf, zzMatrixBaseST * pMatrix);

#endif /* INCLUDED_ZZ_TASK_DEFS_H */

