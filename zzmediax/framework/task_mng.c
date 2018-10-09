/* task_mng.c
 *
 */

#include "task_mng.h"
#include "event.h"
#include "matrix_base.h"

zzStatus ZZTaskMng_Init(zzTaskMngST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    pSelf->curr_task = NULL;

    return sts;
}

zzStatus ZZTaskMng_Execute(zzTaskMngST *pSelf, zzU16 argc, zz_char **argv, zzTaskBaseST *pFlowInfo)
{
    zzStatus sts      = ZZ_ERR_NONE;

    pSelf->curr_task = pFlowInfo;

    //Task PreExec
    if (NULL != pSelf->curr_task->pfnZZTaskPreExec)
    {
        sts = pSelf->curr_task->pfnZZTaskPreExec(pSelf->curr_task, argc, argv);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("Task PreExec error\n");
            goto END;
        }
    }

    //execute matrix
    while (TRUE)
    {
        pSelf->curr_task->task_alive = FALSE;

        //ExecInit Task
        if (NULL != pSelf->curr_task->pfnZZTaskExecInit)
        {
            sts = pSelf->curr_task->pfnZZTaskExecInit(pSelf->curr_task);
            if (sts != ZZ_ERR_NONE)
            {
                ZZPRINTF("Task Exec Init error\n");
                goto END;
            }
        }

        if (NULL != pSelf->curr_task->pfnZZTaskExec)
        {
            sts = pSelf->curr_task->pfnZZTaskExec(pSelf->curr_task);
            if (sts != ZZ_ERR_NONE)
            {
                ZZPRINTF("Task Exec error\n");
                goto END;
            }
        }

        //gest task alive status
        if (FALSE == pSelf->curr_task->task_alive)
        {
            break;
        }
    }

    //Task PostExec
    if (NULL != pSelf->curr_task->pfnZZTaskPreExec)
    {
        sts = pSelf->curr_task->pfnZZTaskPostExec(pSelf->curr_task);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("Task PostExec error\n");
            goto END;
        }
    }

END:
    return sts;
}

zzStatus ZZTaskMng_Close(zzTaskMngST *pSelf)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    return sts;
}

zzBOOL ZZTaskMng_CheckTaskInExecList(zzTaskMngST *pSelf, zzU16 taskID)
{
    zzBOOL    sts = FALSE;
    zz_list*  pos = NULL;

    list_for_each(pos, &pSelf->exec_task_head)
    {
        zzTaskBaseST *pTask = container_of(pos, zzTaskBaseST, task_list);
        if (taskID == pTask->task_id)
        {
            sts = TRUE;
            break;
        }
    }

    return sts;
}

zzStatus ZZTaskMng_StartTask(zzTaskMngST *pSelf, zzU16 argc, zz_char **argv, zzTaskBaseST *pFlowInfo)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    return sts;
}

zzStatus ZZTaskMng_EndTask(zzTaskMngST *pSelf)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    return sts;
}

zzStatus ZZTaskMng_ExecStartTask(zzTaskMngST *pSelf)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    return sts;
}

zzStatus ZZTaskMng_ExecEndTask(zzTaskMngST *pSelf)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    return sts;
}
