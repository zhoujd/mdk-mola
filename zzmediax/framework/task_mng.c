/// task_mng.c

#include "task_mng.h"
#include "event.h"
#include "matrix_base.h"


static zzStatus ZZTaskMng_ExecuteMatrix(zzTaskMngST *pSelf);

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

    //Init Task
    if (NULL != pSelf->curr_task->pfnZZTaskInit)
    {
        sts = pSelf->curr_task->pfnZZTaskInit(pSelf->curr_task, argc, argv);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("Task Init error\n");
            goto END;
        }
    }

    //execute matrix
    while (TRUE)
    {
        pSelf->curr_task_alive = FALSE;

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

        sts = ZZTaskMng_ExecuteMatrix(pSelf);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZTaskMng_ExecuteMatrix error\n");
            goto END;
        }

        //gest task alive status
        if (FALSE == pSelf->curr_task_alive)
        {
            break;
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

zzStatus ZZTaskMng_ExecuteMatrix(zzTaskMngST *pSelf)
{
    zzStatus        sts        = ZZ_ERR_NONE;
    zz_list         *pos       = NULL;
    zzMatrixBaseST  *pMatrix   = NULL;

    //loop martrixs
    list_for_each(pos, &pSelf->curr_task->matrix_head)
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
            ZZPRINTF("Skip Matrix for it is done: %d\n", pMatrix->matrix_id);
            continue;   //end loop
        }

        pSelf->curr_task_alive = TRUE;

        //init Matrix start event
        if (FALSE == pMatrix->restart_flag)
        {
            pMatrix->next_event = ZZ_EVENT_START;
            pMatrix->restart_flag = TRUE;
        }
        else
        {
            pMatrix->next_event = ZZ_EVENT_PART_START;
        }

        while (TRUE)
        {
            zzMatrixCellST     *pCell   = NULL;

            if (ZZ_EVENT_END == pMatrix->next_event)
            {
                ZZPRINTF("This Matrix is done: %d\n", pMatrix->matrix_id);
                break;
            }
            else if (ZZ_EVENT_PART_END == pMatrix->next_event)
            {
                ZZPRINTF("This Matrix is part done: %d\n", pMatrix->matrix_id);
                break;
            }

            sts = ZZMatrixBase_FindCell(pMatrix, pMatrix->next_event, &pCell);
            if (sts != ZZ_ERR_NONE)
            {
                ZZPRINTF("ZZMatrixBase_FindCell error\n");
                goto END;
            }

            //prepare cell execute
            if (NULL != pCell->pfnPreExec)
            {
                sts = pCell->pfnPreExec(pMatrix);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("Cell PreExec error\n");
                    goto END;
                }
            }

            //run cell function
            if (NULL != pCell->pfnExec)
            {
                sts = pCell->pfnExec(pMatrix);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("Cell Exec error\n");
                    goto END;
                }
            }
            else
            {
                ZZPRINTF("Cell Exec is must setting\n");
                sts = ZZ_ERR_UNSUPPORTED;
                goto END;
            }

            //post cell execute
            if (NULL != pCell->pfnPostExec)
            {
                sts = pCell->pfnPostExec(pMatrix);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("Cell PostExec error\n");
                    goto END;
                }
            }
        }
    }

END:
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
