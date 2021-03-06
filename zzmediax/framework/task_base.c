/* task_base.c
 *
 */

#include "task_base.h"
#include "matrix_base.h"

static zzStatus ZZTaskBase_ParseInputString(zzTaskBaseST  *pSelf, int nArgNum, char **strInput);

zzStatus ZZTaskBase_RegisterFP(zzTaskBaseST         *pSelf,
                               zzTaskInitFn         pfnZZTaskInit,
                               zzTaskExecInitFn     pfnZZTaskExecInit,
                               zzTaskPreExecFn      pfnZZTaskPreExec,
                               zzTaskExecFn         pfnZZTaskExec,
                               zzTaskPostExecFn     pfnZZTaskPostExec,
                               zzTaskReleaseFn      pfnZZTaskRelease,
                               zzTaskHelpFn         pfnZZTaskHelp)
{
    zzStatus  sts = ZZ_ERR_NONE;

    pSelf->pfnZZTaskInit        = pfnZZTaskInit;
    pSelf->pfnZZTaskExecInit    = pfnZZTaskExecInit;
    pSelf->pfnZZTaskPreExec     = pfnZZTaskPreExec;
    pSelf->pfnZZTaskExec        = pfnZZTaskExec;
    pSelf->pfnZZTaskPostExec    = pfnZZTaskPostExec;
    pSelf->pfnZZTaskRelease     = pfnZZTaskRelease;
    pSelf->pfnZZTaskHelp        = pfnZZTaskHelp;

    return sts;

}

zzStatus ZZTaskBase_DefaultFP(zzTaskBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    pSelf->pfnZZTaskInit        = ZZTaskBase_Init;
    pSelf->pfnZZTaskExecInit    = ZZTaskBase_ExecInit;
    pSelf->pfnZZTaskPreExec     = ZZTaskBase_PreExec;
    pSelf->pfnZZTaskExec        = ZZTaskBase_Exec;
    pSelf->pfnZZTaskPostExec    = ZZTaskBase_PostExec;
    pSelf->pfnZZTaskRelease     = ZZTaskBase_Release;
    pSelf->pfnZZTaskRelease     = ZZTaskBase_Release;
    pSelf->pfnZZTaskHelp        = ZZTaskBase_Help;

    return sts;
}

zzStatus ZZTaskBase_Init(zzTaskBaseST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts = ZZ_ERR_NONE;

    INIT_LIST_HEAD(&pSelf->matrix_head);

    sts = ZZTaskBase_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_ParseInputString  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZTaskBase_ExecInit(zzTaskBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    return sts;

}

zzStatus ZZTaskBase_PreExec(zzTaskBaseST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts = ZZ_ERR_NONE;

    return sts;
}

zzStatus ZZTaskBase_Exec(zzTaskBaseST *pSelf)
{
    zzStatus        sts        = ZZ_ERR_NONE;
    zz_list         *pos       = NULL;
    zzMatrixBaseST  *pMatrix   = NULL;

    //loop martrixs
    list_for_each(pos, &pSelf->matrix_head)
    {
        pMatrix = container_of(pos, zzMatrixBaseST, matrix_list);
        //skip invalide task entry
        if (pMatrix == NULL)
        {
            ZZPRINTF("Can not get a Matrix, exist the task\n");
            sts = ZZ_ERR_UNSUPPORTED;
            goto END;
        }

        if (ZZ_EVENT_END == pMatrix->next_event)
        {
            ZZDEBUG("Skip Matrix for it is done: %d\n", pMatrix->matrix_id);
            continue;
        }

        pSelf->task_alive = TRUE;

        sts = ZZMatrixBase_Run(pMatrix);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF(" ZZMatrixBase_Run error\n");
            goto END;
        }
    }

END:
    return sts;
}


zzStatus ZZTaskBase_PostExec(zzTaskBaseST *pSelf)
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

    //free matrix list
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

zzStatus ZZTaskBase_ParseInputString(zzTaskBaseST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;
    zzU8               i       = 1;

    for (i = 1; i < nArgNum; i++ )
    {
        CHECK_POINTER(strInput[i], ZZ_ERR_NULL_PTR);
        {
        }
    }

    return sts;
}
