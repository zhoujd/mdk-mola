/* dispatch.c
 *
 */

#include "dispatch.h"
#include "app_base.h"

zzStatus ZZDispatch_Init(zzDispatch *pSelf, zzU16 argc, zz_char **argv, zzSelectFlowID  pfnSelectFlowID)
{
    zzStatus                sts         = ZZ_ERR_NONE;

    pSelf->pfnSelectFlowID = pfnSelectFlowID;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    return sts;
}

zzStatus ZZDispatch_Close(zzDispatch *pSelf)
{
    zzStatus                sts         = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    return sts;
}

zzStatus ZZDispatch_Run(zzDispatch *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus                sts         = ZZ_ERR_NONE;
    zzU16                   task_id     = 0;
    zzTaskBaseST    *pFlowInfo  = NULL;

    //select workflow
    if (NULL != pSelf->pfnSelectFlowID)
    {
        sts = pSelf->pfnSelectFlowID(GetAPP(), &task_id);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZApp SelectFlowID error\n");
            goto END;
        }
    }

    //select flow to run
    sts = ZZTaskDB_SelectFlow(&GetAPP()->task_db, task_id, &pFlowInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskDB_SelectFlow error\n");
        goto END;
    }

    if (NULL == pFlowInfo)
    {
        ZZPRINTF("pFlowInfo == NULL error\n");
        sts = ZZ_ERR_UNKNOWN;
        goto END;
    }

    //execute task
    sts = ZZTaskMng_Execute(&GetAPP()->task_mng, argc, argv, pFlowInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskDB_SelectFlow error\n");
        goto END;
    }

END:
    return sts;
}
