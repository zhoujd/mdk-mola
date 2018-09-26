
#include "task_base.h"



zzStatus ZZTaskBase_RegisterFP(zzTaskBaseST         *pSelf,
                               zzTaskInitFn         pfnZZTaskInit,
                               zzTaskExecInitFn     pfnZZTaskExecInit,
                               zzTaskReleaseFn      pfnZZTaskRelease,
                               zzTaskHelpFn         pfnZZTaskHelp)
{
    zzStatus  sts = ZZ_ERR_NONE;

    pSelf->pfnZZTaskInit        = pfnZZTaskInit;
    pSelf->pfnZZTaskExecInit    = pfnZZTaskExecInit;
    pSelf->pfnZZTaskRelease     = pfnZZTaskRelease;
    pSelf->pfnZZTaskHelp        = pfnZZTaskHelp;
    
    return sts;
    
}

zzStatus ZZTaskBase_DefaultFP(zzTaskBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    pSelf->pfnZZTaskInit        = ZZTaskBase_Init;
    pSelf->pfnZZTaskExecInit    = ZZTaskBase_ExecInit;
    pSelf->pfnZZTaskRelease     = ZZTaskBase_Release;
    pSelf->pfnZZTaskHelp        = ZZTaskBase_Help;

    return sts;

}

zzStatus ZZTaskBase_Init(zzTaskBaseST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts = ZZ_ERR_NONE;

    INIT_LIST_HEAD(&pSelf->matrix_head);
    
    return sts;
}

zzStatus ZZTaskBase_ExecInit(zzTaskBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    return sts;
    
}

zzStatus ZZTaskBase_Release(zzTaskBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;
    zz_list   *pos   = NULL;
    zz_list   *n     = NULL; 

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);
    
    //free task list
    list_for_each_safe(pos, n, &pSelf->matrix_head)
    {
        zzMatrixBaseST *pTemp = container_of(pos, zzMatrixBaseST, matrix_list);
        list_del_init(pos);
        FREEIF(pTemp);
    }

    return sts;
}

zzStatus ZZTaskBase_Help(zzTaskBaseST *pSelf,  zzU16 argc, zz_char **argv)
{
    zzStatus  sts = ZZ_ERR_NONE;

    return sts;
}

zzStatus ZZTaskBase_AttachMatrix(zzTaskBaseST *pSelf, zzMatrixBaseST *pMatrix)
{
    zzStatus  sts = ZZ_ERR_NONE;

    list_add_tail(&pMatrix->matrix_list, &pSelf->matrix_head);

    return sts;
}