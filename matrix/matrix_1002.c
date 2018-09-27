/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)matrix_1002.c
 */

#include "app_base.h"
#include "matrix_1002.h"
#include "task_base.h"

static zzStatus ZZMatrix1002_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix1002_PartStart(zzMatrixBaseST *pSelf);

static zzStatus ZZMatrix1002_ParseInputString(zzMatrix1002ST  *pSelf, int nArgNum, char **strInput);

//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,         NULL, ZZMatrix1002_Start,      NULL},
    {ZZ_EVENT_PART_START,    NULL, ZZMatrix1002_PartStart,  NULL},
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

    sts = ZZFrameReader_Create(&(*ppRet)->pFrameReader);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZFrameReader_Create error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix1002_Release(zzMatrix1002ST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZFrameReader_Release(pSelf->pFrameReader);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZFrameReader_Release error\n");
        goto END;
    }

END:
    return sts;
}


zzStatus ZZMatrix1002_Init(zzMatrix1002ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf, zz_char *pFilename)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZMatrix1002_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1002_ParseInputString error\n");
        goto END;
    }

    pSelf->base.pipe_ctrl = pPipeCtrl;
    pSelf->dst_surf       = *pSurf;

    sts = ZZFrameReader_Init(pSelf->pFrameReader, pFilename);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZFrameReader_Init error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix1002_Start(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix1002ST *pSelf = GET_MATRIX1002(pMatrixBase);


    ZZPRINTF("Matrix %d Start\n", pSelf->base.matrix_id);

    sts = ZZSurface_GetNextInputFrame(&pSelf->dst_surf, pSelf->pFrameReader);
    switch (sts)
    {
    case ZZ_ERR_NONE:
        pMatrixBase->next_event = ZZ_EVENT_PART_END;
        break;
    case ZZ_ERR_EOF_STREAM:
        pMatrixBase->next_event = ZZ_EVENT_END;
        pSelf->base.pipe_ctrl->pipe_event = ZZ_EVENT_PIPE_EXIT;
        sts = ZZ_ERR_NONE;
        break;
    default:
        ZZPRINTF("ZZ_GetNextInputFrame error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix1002_PartStart(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix1002ST *pSelf = GET_MATRIX1002(pMatrixBase);

    ZZPRINTF("Matrix %d PartStart\n", pSelf->base.matrix_id);

    sts = ZZSurface_GetNextInputFrame(&pSelf->dst_surf, pSelf->pFrameReader);
    switch (sts)
    {
    case ZZ_ERR_NONE:
        pMatrixBase->next_event = ZZ_EVENT_PART_END;
        break;
    case ZZ_ERR_EOF_STREAM:
        pMatrixBase->next_event = ZZ_EVENT_END;
        pSelf->base.pipe_ctrl->pipe_event = ZZ_EVENT_PIPE_EXIT;
        sts = ZZ_ERR_NONE;
        break;
    default:
        ZZPRINTF("ZZ_GetNextInputFrame error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix1002_ParseInputString(zzMatrix1002ST  *pSelf, int nArgNum, char **strInput)
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
