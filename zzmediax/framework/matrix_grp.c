/* matrix_grp.c
 *
 */

#include "app_base.h"
#include "matrix_grp.h"
#include "task_base.h"

static zzStatus ZZMatrixGRP_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrixGRP_PartStart(zzMatrixBaseST *pSelf);

static zzStatus ZZMatrixGRP_ParseInputString(zzMatrixGRPST  *pSelf, int nArgNum, char **strInput);

//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,         NULL, ZZMatrixGRP_Start,      NULL},
    {ZZ_EVENT_PART_START,    NULL, ZZMatrixGRP_PartStart,  NULL},
};

zzStatus ZZMatrixGRP_Create(zzMatrixGRPST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;

    *ppRet = (zzMatrixGRPST *)AllocAndZeroMem(sizeof(zzMatrixGRPST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);
    (*ppRet)->base.matrix_id = MATRIXGRP_ID;


END:
    return sts;
}

zzStatus ZZMatrixGRP_Release(zzMatrixGRPST *pSelf)
{
    zzStatus  sts    = ZZ_ERR_NONE;
    zz_list   *pos   = NULL;
    zz_list   *n     = NULL;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    list_for_each_safe(pos, n, &pSelf->matrix_head)
    {
        list_del_init(pos);
    }

    return sts;
}

zzStatus ZZMatrixGRP_Init(zzMatrixGRPST *pSelf, zzU16 argc, zz_char **argv,
                          zzPipeCtrlST  *pPipeCtrl)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    INIT_LIST_HEAD(&pSelf->matrix_head);

    sts = ZZMatrixGRP_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrixGRP_ParseInputString error\n");
        goto END;
    }


END:
    return sts;
}

zzStatus ZZMatrixGRP_AttachMatrix(zzMatrixGRPST *pSelf, zzMatrixBaseST *pMatrix)
{
    zzStatus  sts = ZZ_ERR_NONE;

    list_add_tail(&pMatrix->matrix_list, &pSelf->matrix_head);

    return sts;
}

zzStatus ZZMatrixGRP_Start(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzMatrixGRPST *pSelf = GET_MATRIXGRP(pMatrixBase);

    ZZDEBUG("Matrix %d Start\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_PART_END;

    return sts;
}

zzStatus ZZMatrixGRP_PartStart(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzMatrixGRPST *pSelf = GET_MATRIXGRP(pMatrixBase);

    ZZDEBUG("Matrix %d PartStart\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_END;

    return sts;
}

zzStatus ZZMatrixGRP_ParseInputString(zzMatrixGRPST  *pSelf, int nArgNum, char **strInput)
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
