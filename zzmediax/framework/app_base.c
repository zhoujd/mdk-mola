/* app_base.c
 *
 */


#include "app_base.h"


zzStatus ZZApp_Init(zzAppST            *pSelf,
                    zzU16              argc,
                    zz_char            **argv,
                    zzSelectTaskIdFP   pfnSelectFlowID,
                    zzTaskRegisterFP   pfnTaskRegister,
                    zzHelpRegisterFP   pfnHelpRegister)
{
    zzStatus  sts = ZZ_ERR_NONE;

    //parse Input param
    sts = ZZApp_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_ParseInputString error\n");
        goto END;
    }

    //init workflow DB
    sts = ZZTaskDB_Init(&pSelf->task_db, argc, argv, pfnTaskRegister);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskDB_Init error\n");
        goto END;
    }

    //init dispatch
    sts = ZZDispatch_Init(&pSelf->dispatch, argc, argv, pfnSelectFlowID);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskDB_Init error\n");
        goto END;
    }

    //init task manage
    sts = ZZTaskMng_Init(&pSelf->task_mng, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskDB_Init error\n");
        goto END;
    }

    //init help
    sts = ZZHelp_Init(&pSelf->help, argc, argv, pfnHelpRegister);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZHelp_Init error\n");
        goto END;
    }


END:
    return ZZ_ERR_NONE;
}

zzStatus ZZApp_Close(zzAppST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    //close task manage
    sts = ZZTaskMng_Close(&pSelf->task_mng);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskMng_Close error\n");
        goto END;
    }

    //close dispatch
    sts = ZZDispatch_Close(&pSelf->dispatch);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZDispatch_Close error\n");
        goto END;
    }

    //close task DB
    sts = ZZTaskDB_Close(&pSelf->task_db);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskDB_Init error\n");
        goto END;
    }

    //close help
    sts = ZZHelp_Close(&pSelf->help);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZHelp_Close error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZApp_Execute(zzAppST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts = ZZ_ERR_NONE;

    sts = ZZDispatch_Run(&GetAPP()->dispatch, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZDispatch_Run  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZApp_PrintHelp(zzAppST  *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    return sts;
}

zzStatus ZZApp_ParseInputString(zzAppST *pSelf, zzU16 argc, zz_char ** argv)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    //TODO
    return sts;
}

zzStatus ZZApp_AddFlow(zzAppST *pSelf, zzTaskBaseST *pFlowInfo)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZTaskDB_AddFlow(&pSelf->task_db, pFlowInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskDB_AddFlow  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZApp_TaskHelp(zzAppST               *pSelf,
                        zzU16                 argc,
                        zz_char               **argv,
                        zzU16                 taskID)
{
    zzStatus     sts        = ZZ_ERR_NONE;
    zzTaskBaseST *pFlowInfo = NULL;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZTaskDB_SelectFlow(&pSelf->task_db, taskID, &pFlowInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskDB_SelectFlow  error\n");
        goto END;
    }

    if (NULL != pFlowInfo->pfnZZTaskHelp)
    {
        sts = pFlowInfo->pfnZZTaskHelp(pFlowInfo, argc, argv);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("pFlowInfo->pfnZZTaskHelp  error\n");
            goto END;
        }
    }

END:
    return sts;
}


zzStatus ZZApp_MatrixHelp(zzAppST               *pSelf,
                          zzU16                  argc,
                          zz_char                **argv,
                          zzU16                  matrix_id)
{
    return ZZHelp_MatrixHelp(&pSelf->help, argc, argv, matrix_id);
}
