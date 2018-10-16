/* matrix_base.c
 *
 */

#include "matrix_base.h"

static zzStatus ZZMatrixBase_FindCell(zzMatrixBaseST *pSelf, zzEventID id, zzMatrixCellST **ppCell);


zzStatus ZZMatrixBase_Run(zzMatrixBaseST *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    //init Matrix start event
    if (FALSE == pSelf->restart_flag)
    {
        pSelf->next_event   = ZZ_EVENT_START;
        pSelf->restart_flag = TRUE;
    }
    else
    {
        pSelf->next_event = ZZ_EVENT_PART_START;
    }

    while (TRUE)
    {
        zzMatrixCellST     *pCell   = NULL;

        if (ZZ_EVENT_END == pSelf->next_event)
        {
            ZZDEBUG("This Matrix is done: %d\n", pSelf->matrix_id);
            break;
        }
        else if (ZZ_EVENT_PART_END == pSelf->next_event)
        {
            ZZDEBUG("This Matrix is part done: %d\n", pSelf->matrix_id);
            break;
        }

        sts = ZZMatrixBase_FindCell(pSelf, pSelf->next_event, &pCell);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZMatrixBase_FindCell error\n");
            goto END;
        }

        //prepare cell execute
        if (NULL != pCell->pfnPreExec)
        {
            sts = pCell->pfnPreExec(pSelf);
            if (sts != ZZ_ERR_NONE)
            {
                ZZPRINTF("Cell PreExec error\n");
                goto END;
            }
        }

        //run cell function
        if (NULL != pCell->pfnExec)
        {
            sts = pCell->pfnExec(pSelf);
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
            sts = pCell->pfnPostExec(pSelf);
            if (sts != ZZ_ERR_NONE)
            {
                ZZPRINTF("Cell PostExec error\n");
                goto END;
            }
        }
    }

END:
    return sts;
}

zzStatus ZZMatrixBase_FindCell(zzMatrixBaseST *pSelf, zzEventID id, zzMatrixCellST **ppCell)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrixCellST *pTemp = NULL;
    zzU16          i      = 0;
    zzBOOL         bFound = FALSE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    for (i = 0; i < pSelf->cell_num; i++)
    {
        if (id == pSelf->cell_list[i].id)
        {
            pTemp = &pSelf->cell_list[i];
            bFound = TRUE;
            break;
        }
    }

    if (TRUE == bFound)
    {
        *ppCell = pTemp;
    }
    else
    {
        sts = ZZ_ERR_NOT_FOUND;
    }

    return sts;
}

zzStatus ZZMatrixBase_Release(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);
    //TODO

    return sts;
}
