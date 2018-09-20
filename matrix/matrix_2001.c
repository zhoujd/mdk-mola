/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)zz_proc_2001.c
 */

#include "app_base.h"
#include "matrix_2001.h"
#include "task_base.h"


static zzStatus ZZMatrix2001_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix2001_Test(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix2001_End(zzMatrixBaseST *pSelf);

enum MATRIX2001_EVENT_EN
{
    ZZ_EVENT_TASK2001_DONE = ZZ_EVENT_USER + 1,
    ZZ_EVENT_TASK2001_TEST,
};

//Matrix Cells
static zzMatrixCellST matrix_cells[] = 
{
    {ZZ_EVENT_START,         NULL,  ZZMatrix2001_Start, NULL},      //ZZ_EVENT_START
    {ZZ_EVENT_TASK2001_TEST, NULL,  ZZMatrix2001_Test,  NULL },     //ZZ_EVENT_TASK2001_TEST
    {ZZ_EVENT_TASK2001_DONE, NULL,  ZZMatrix2001_End,   NULL},      //ZZ_EVENT_TASK2001_DONE
};


zzStatus ZZMatrix2001_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 2001 help\n");

    return sts;    
}

zzStatus ZZMatrix2001_HelpRegister()
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

    pHelpInfo->help_id         = MATRIX2001_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix2001_Help;
    
    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;    
}

zzStatus ZZMatrix2001_Create(zzMatrix2001ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;
    
    *ppRet = (zzMatrix2001ST *)AllocAndZeroMem(sizeof(zzMatrix2001ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);

    (*ppRet)->base.matrix_id = MATRIX2001_ID;
        
END:
    return sts;    
}

zzStatus ZZMatrix2001_Release(zzMatrix2001ST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);
   
    FREEIF(pSelf);

    return sts;    
}

zzStatus ZZMatrix2001_Start(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;
    
    ZZPRINTF("Matrix 2001 Start\n");

    pSelf->next_event = ZZ_EVENT_TASK2001_TEST;
    
    return sts;
}

zzStatus ZZMatrix2001_Test(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 2001 Test\n");

    pSelf->next_event = ZZ_EVENT_TASK2001_DONE;


    return sts;
}

zzStatus ZZMatrix2001_End(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 2001 End\n");

    pSelf->next_event = ZZ_EVENT_END;
    

    return sts;
}
