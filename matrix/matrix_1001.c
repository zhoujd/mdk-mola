/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)zz_proc_1001.c
 */

#include "app_base.h"
#include "matrix_1001.h"
#include "task_base.h"


static zzStatus ZZMatrix1001_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix1001_Test(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix1001_End(zzMatrixBaseST *pSelf);

enum MATRIX1001_EVENT_EN
{
    ZZ_EVENT_TASK1001_DONE = ZZ_EVENT_USER + 1,
    ZZ_EVENT_TASK1001_TEST,
};

//Matrix Cells
static zzMatrixCellST matrix_cells[] = 
{
    {ZZ_EVENT_START,         NULL,  ZZMatrix1001_Start, NULL},      //ZZ_EVENT_START
    {ZZ_EVENT_TASK1001_TEST, NULL,  ZZMatrix1001_Test,  NULL },     //ZZ_EVENT_TASK1001_TEST
    {ZZ_EVENT_TASK1001_DONE, NULL,  ZZMatrix1001_End,   NULL},      //ZZ_EVENT_TASK1001_DONE
};


zzStatus ZZMatrix1001_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 1001 help\n");

    return sts;    
}

zzStatus ZZMatrix1001_HelpRegister()
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

    pHelpInfo->help_id         = MATRIX1001_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix1001_Help;
    
    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;    
}

zzStatus ZZMatrix1001_Create(zzMatrix1001ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;
    
    *ppRet = (zzMatrix1001ST *)AllocAndZeroMem(sizeof(zzMatrix1001ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);

    (*ppRet)->base.matrix_id = MATRIX1001_ID;
        
END:
    return sts;    
}

zzStatus ZZMatrix1001_Release(zzMatrix1001ST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);
   
    FREEIF(pSelf);

    return sts;    
}

zzStatus ZZMatrix1001_Start(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;
    
    ZZPRINTF("Matrix 1001 Start\n");

    pSelf->next_event = ZZ_EVENT_TASK1001_TEST;
    
    return sts;
}

zzStatus ZZMatrix1001_Test(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 1001 Test\n");

    pSelf->next_event = ZZ_EVENT_TASK1001_DONE;


    return sts;
}

zzStatus ZZMatrix1001_End(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 1001 End\n");

    pSelf->next_event = ZZ_EVENT_END;
    

    return sts;
}
