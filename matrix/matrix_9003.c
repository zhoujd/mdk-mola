/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)zz_proc_9003.c
 */

#include "app_base.h"
#include "matrix_9003.h"
#include "task_base.h"

static zzStatus ZZMatrix9003_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix9003_PartStart(zzMatrixBaseST *pSelf);

//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,         NULL,  ZZMatrix9003_Start,      NULL},
    {ZZ_EVENT_PART_START,    NULL,  ZZMatrix9003_PartStart,  NULL},
};


zzStatus ZZMatrix9003_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 9003 : output frame to file\n");

    return sts;
}

zzStatus ZZMatrix9003_HelpRegister()
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

    pHelpInfo->help_id         = MATRIX9003_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix9003_Help;

    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix9003_Create(zzMatrix9003ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;

    *ppRet = (zzMatrix9003ST *)AllocAndZeroMem(sizeof(zzMatrix9003ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);

    (*ppRet)->base.matrix_id = MATRIX9003_ID;

    sts = ZZFrameWriter_Create(&(*ppRet)->pFrameWriter);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZFrameWriter_Create error\n");
        goto END;
    }

END:
    return sts;
}


zzStatus ZZMatrix9003_Init(zzMatrix9003ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf, zz_char *pFilename)
{
    zzStatus  sts = ZZ_ERR_NONE;

    pSelf->base.pipe_ctrl = pPipeCtrl;
    pSelf->src_surf       = pSurf;

    sts = ZZFrameWriter_Init(pSelf->pFrameWriter, pFilename);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZFrameWriter_Init error\n");
        goto END;
    }

END:
    return sts;
}


zzStatus ZZMatrix9003_Release(zzMatrix9003ST *pSelf)
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

zzStatus ZZMatrix9003_Start(zzMatrixBaseST *pMatrixBase)
{
    zzStatus        sts   = ZZ_ERR_NONE;
    zzMatrix9003ST *pSelf = GET_MATRIX9003(pMatrixBase);

    ZZDEBUG("Matrix 9003 Start\n");

    if (pSelf->base.pipe_ctrl->pipe_event == ZZ_EVENT_PIPE_EXIT)
    {
        ZZDEBUG("(%s) get exist event\n", __FUNCTION__);
        pMatrixBase->next_event = ZZ_EVENT_END;
        goto END;
    }

    sts = ZZSurface_WriteOutputFrame(pSelf->src_surf, pSelf->pFrameWriter);
    switch (sts)
    {
    case ZZ_ERR_NONE:
        pMatrixBase->next_event = ZZ_EVENT_PART_END;
        break;
    default:
        ZZPRINTF("ZZ_GetNextInputFrame error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix9003_PartStart(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix9003ST *pSelf = GET_MATRIX9003(pMatrixBase);

    //ZZPRINTF("Matrix 9003 PartStart\n");

    if (pSelf->base.pipe_ctrl->pipe_event == ZZ_EVENT_PIPE_EXIT)
    {
        //ZZPRINTF("(%s) get exist event\n", __FUNCTION__);
        pMatrixBase->next_event = ZZ_EVENT_END;
        goto END;
    }

    sts = ZZSurface_WriteOutputFrame(pSelf->src_surf, pSelf->pFrameWriter);
    switch (sts)
    {
    case ZZ_ERR_NONE:
        pMatrixBase->next_event = ZZ_EVENT_PART_END;
        break;
    default:
        ZZPRINTF("ZZ_GetNextInputFrame error\n");
        goto END;
    }

END:
    return sts;
}
