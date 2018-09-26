/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)matrix_1002.c
 */

#include "app_base.h"
#include "matrix_1002.h"
#include "task_base.h"

static zzStatus ZZMatrix1002_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix1002_Test(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix1002_End(zzMatrixBaseST *pSelf);

enum MATRIX1002_EVENT_EN
{
    ZZ_EVENT_TASK1002_TEST = ZZ_EVENT_USER + 1,
    ZZ_EVENT_TASK1002_DONE,
};

//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,         NULL, ZZMatrix1002_Start, NULL},     //ZZ_EVENT_START
    {ZZ_EVENT_TASK1002_TEST, NULL, ZZMatrix1002_Test,  NULL},     //ZZ_EVENT_TEST
    {ZZ_EVENT_TASK1002_DONE, NULL, ZZMatrix1002_End,   NULL},     //ZZ_EVENT_END
};


zzStatus ZZMatrix1002_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 1002 help\n");

    return sts;
}

zzStatus ZZMatrix1002_HelpRegister()
{
    zzStatus  sts    = ZZ_ERR_NONE;
    zzAppST   *pApp  = GetAPP();

    zzHelpInfoST  *pHelpInfo = NULL;

    pHelpInfo = (zzHelpInfoST *)AllocAndZeroMem(sizeof(zzHelpInfoST));
    if (pHelpInfo == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    pHelpInfo->help_id         = MATRIX1002_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix1002_Help;

    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix1002_Create(zzMatrix1002ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;

    *ppRet = (zzMatrix1002ST *)AllocAndZeroMem(sizeof(zzMatrix1002ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);

    (*ppRet)->base.matrix_id     = MATRIX1002_ID;

END:
    return sts;
}

zzStatus ZZMatrix1002_Release(zzMatrix1002ST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    FREEIF(pSelf);

    return sts;
}


zzStatus ZZMatrix1002_Init(zzMatrix1002ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST *pPipeCtrl)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    pSelf->base.pipe_ctrl = pPipeCtrl;

    FREEIF(pSelf);

    return sts;
}

zzStatus ZZMatrix1002_Start(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix1002ST *pSelf = GET_MATRIX1002(pMatrixBase);


    ZZPRINTF("Matrix %d Start\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_TASK1002_TEST;

    return sts;
}

zzStatus ZZMatrix1002_Test(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix1002ST *pSelf = GET_MATRIX1002(pMatrixBase);

    ZZPRINTF("Matrix %d Test\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_TASK1002_DONE;

    return sts;

}


zzStatus ZZMatrix1002_End(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix1002ST *pSelf = GET_MATRIX1002(pMatrixBase);

    ZZPRINTF("Matrix %d End\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_END;

    return sts;

}
