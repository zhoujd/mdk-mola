/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)matrix_2002.c
 */

#include "app_base.h"
#include "matrix_2002.h"
#include "task_base.h"

static zzStatus ZZMatrix2002_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix2002_Test(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix2002_End(zzMatrixBaseST *pSelf);

static zzStatus ZZMatrix2002_ParseInputString(zzMatrix2002ST  *pSelf, int nArgNum, char **strInput);

enum MATRIX2002_EVENT_EN
{
    ZZ_EVENT_TASK2002_TEST = ZZ_EVENT_USER + 1,
    ZZ_EVENT_TASK2002_DONE,
};

//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,         NULL, ZZMatrix2002_Start, NULL},     //ZZ_EVENT_START
    {ZZ_EVENT_TASK2002_TEST, NULL, ZZMatrix2002_Test,  NULL},     //ZZ_EVENT_TEST
    {ZZ_EVENT_TASK2002_DONE, NULL, ZZMatrix2002_End,   NULL},     //ZZ_EVENT_END
};


zzStatus ZZMatrix2002_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 2002 help\n");

    return sts;
}

zzStatus ZZMatrix2002_HelpRegister()
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

    pHelpInfo->help_id         = MATRIX2002_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix2002_Help;

    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix2002_Create(zzMatrix2002ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;

    *ppRet = (zzMatrix2002ST *)AllocAndZeroMem(sizeof(zzMatrix2002ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);

    (*ppRet)->base.matrix_id     = MATRIX2002_ID;

END:
    return sts;
}

zzStatus ZZMatrix2002_Release(zzMatrix2002ST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    FREEIF(pSelf);

    return sts;
}


zzStatus ZZMatrix2002_Init(zzMatrix2002ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST *pPipeCtrl)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZMatrix2002_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2002_ParseInputString error\n");
        goto END;
    }

    pSelf->base.pipe_ctrl = pPipeCtrl;

    FREEIF(pSelf);

END:
    return sts;
}

zzStatus ZZMatrix2002_Start(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix2002ST *pSelf = GET_MATRIX2002(pMatrixBase);


    ZZPRINTF("Matrix %d Start\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_TASK2002_TEST;

    return sts;
}

zzStatus ZZMatrix2002_Test(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix2002ST *pSelf = GET_MATRIX2002(pMatrixBase);

    ZZPRINTF("Matrix %d Test\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_TASK2002_DONE;

    return sts;

}


zzStatus ZZMatrix2002_End(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix2002ST *pSelf = GET_MATRIX2002(pMatrixBase);

    ZZPRINTF("Matrix %d End\n", pSelf->base.matrix_id);

    pMatrixBase->next_event = ZZ_EVENT_END;

    return sts;

}

zzStatus ZZMatrix2002_ParseInputString(zzMatrix2002ST  *pSelf, int nArgNum, char **strInput)
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
