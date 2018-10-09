/* matrix_base.c
 *
 */

#include "matrix_base.h"

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
