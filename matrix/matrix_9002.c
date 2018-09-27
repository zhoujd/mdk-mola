/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)zz_proc_9002.c
 */

#include "app_base.h"
#include "matrix_9002.h"
#include "task_base.h"


static zzStatus ZZMatrix9002_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix9002_Test(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix9002_End(zzMatrixBaseST *pSelf);

enum MATRIX9002_EVENT_EN
{
    ZZ_EVENT_TASK9002_DONE = ZZ_EVENT_USER + 1,
    ZZ_EVENT_TASK9002_TEST,
};

//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,         NULL,  ZZMatrix9002_Start, NULL},      //ZZ_EVENT_START
    {ZZ_EVENT_TASK9002_TEST, NULL,  ZZMatrix9002_Test,  NULL },     //ZZ_EVENT_TASK9002_TEST
    {ZZ_EVENT_TASK9002_DONE, NULL,  ZZMatrix9002_End,   NULL},      //ZZ_EVENT_TASK9002_DONE
};


zzStatus ZZMatrix9002_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 9002 help\n");

    return sts;
}

zzStatus ZZMatrix9002_HelpRegister()
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

    pHelpInfo->help_id         = MATRIX9002_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix9002_Help;

    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix9002_Create(zzMatrix9002ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;

    *ppRet = (zzMatrix9002ST *)AllocAndZeroMem(sizeof(zzMatrix9002ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);

    (*ppRet)->base.matrix_id = MATRIX9002_ID;

    sts = ZZFrameWriter_Create(&(*ppRet)->pFrameWriter);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZFrameWriter_Create error\n");
        goto END;
    }

END:
    return sts;
}


zzStatus ZZMatrix9002_Init(zzMatrix9002ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf, zz_char *pFilename)
{
    zzStatus  sts = ZZ_ERR_NONE;

    pSelf->base.pipe_ctrl = pPipeCtrl;
    pSelf->src_surf       = *pSurf;

    sts = ZZFrameWriter_Init(pSelf->pFrameWriter, pFilename);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZFrameWriter_Init error\n");
        goto END;
    }

END:
    return sts;
}


zzStatus ZZMatrix9002_Release(zzMatrix9002ST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZFrameWriter_Release(pSelf->pFrameWriter);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZFrameWriter_Release error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix9002_Start(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 9002 Start\n");

    pSelf->next_event = ZZ_EVENT_TASK9002_TEST;

    return sts;
}

zzStatus ZZMatrix9002_Test(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 9002 Test\n");

    pSelf->next_event = ZZ_EVENT_TASK9002_DONE;


    return sts;
}

zzStatus ZZMatrix9002_End(zzMatrixBaseST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 9002 End\n");

    pSelf->next_event = ZZ_EVENT_END;


    return sts;
}
