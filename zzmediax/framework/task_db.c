/* task_db.c
 *
 */

#include "task_db.h"
#include "app_base.h"


zzStatus ZZTaskDB_Init(zzTaskDBST *pSelf, zzU16 argc, zz_char** argv, zzTaskRegisterFP  pfnTaskRegister)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    INIT_LIST_HEAD(&pSelf->task_head);
    pSelf->pfnTaskRegister = pfnTaskRegister;

    //register task
    if (NULL != pSelf->pfnTaskRegister)
    {
        sts = pSelf->pfnTaskRegister(GetAPP());
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZTaskDB TaskRegister error\n");
            goto END;
        }
    }

    //init flows
    sts = ZZTaskDB_InitFlows(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskDB ZZTaskDB_InitFlows error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZTaskDB_InitFlows(zzTaskDBST *pSelf, zzU16 argc, zz_char** argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;
    zz_list*  pos    = NULL;

    //init task
    list_for_each(pos, &pSelf->task_head)
    {
        zzTaskBaseST *pTemp = container_of(pos, zzTaskBaseST, task_list);
        if (NULL != pTemp)
        {
            if (NULL != pTemp->pfnZZTaskInit)
            {
                sts = pTemp->pfnZZTaskInit(pTemp, argc, argv);
                if (ZZ_ERR_NONE != sts)
                {
                    ZZPRINTF("ZZTaskDB init error\n");
                    goto END;
                }
            }
        }
    }

END:
    return sts;
}

zzStatus ZZTaskDB_AddFlow(zzTaskDBST *pSelf, zzTaskBaseST *pFlowInfo)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pFlowInfo, ZZ_ERR_NOT_INITIALIZED);

    list_add_tail(&pFlowInfo->task_list, &pSelf->task_head);

    return sts;
}

zzStatus ZZTaskDB_SelectFlow(zzTaskDBST *pSelf, zzU16 task_id, zzTaskBaseST **ppFlowInfo)
{
    zzStatus  sts    = ZZ_ERR_NONE;
    zzBOOL    bFound = FALSE;
    zz_list*  pos    = NULL;

    list_for_each(pos, &pSelf->task_head)
    {
        zzTaskBaseST *pTemp = container_of(pos, zzTaskBaseST, task_list);
        if (NULL != pTemp)
        {
            if (pTemp->task_id == task_id)
            {
                *ppFlowInfo = pTemp;
                bFound = TRUE;
                break;
            }
        }
    }

    if (!bFound)
    {
        sts = ZZ_ERR_NOT_FOUND;
        goto END;
    }

END:
    return sts;
}

zzStatus ZZTaskDB_Close(zzTaskDBST *pSelf)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    //Release flows
    sts = ZZTaskDB_ReleaseFlows(pSelf);
    if (ZZ_ERR_NONE != sts)
    {
        ZZPRINTF("ZZTaskDB Release flows error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZTaskDB_ReleaseFlows(zzTaskDBST *pSelf)
{
    zzStatus  sts    = ZZ_ERR_NONE;
    zz_list   *pos   = NULL;
    zz_list   *n     = NULL;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    //free task list
    list_for_each_safe(pos, n, &pSelf->task_head)
    {
        zzTaskBaseST *pTemp = container_of(pos, zzTaskBaseST, task_list);

        if (NULL != pTemp->pfnZZTaskRelease)
        {
            sts = pTemp->pfnZZTaskRelease(pTemp);
            if (ZZ_ERR_NONE != sts)
            {
                ZZPRINTF("ZZTaskDB release error\n");
                goto END;
            }
        }

        list_del_init(pos);
        FREEIF(pTemp);
    }

END:
    return sts;
}
