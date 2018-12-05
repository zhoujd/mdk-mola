/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)matrix_1003.c
 */

#include "app_base.h"
#include "matrix_1003.h"
#include "task_base.h"

static zzStatus ZZMatrix1003_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix1003_PartStart(zzMatrixBaseST *pSelf);

static zzStatus ZZMatrix1003_ParseInputString(zzMatrix1003ST  *pSelf, int nArgNum, char **strInput);

//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,         NULL, ZZMatrix1003_Start,      NULL},
    {ZZ_EVENT_PART_START,    NULL, ZZMatrix1003_PartStart,  NULL},
};


zzStatus ZZMatrix1003_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 1003 help\n");

    return sts;
}

zzStatus ZZMatrix1003_HelpRegister()
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

    pHelpInfo->help_id         = MATRIX1003_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix1003_Help;

    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix1003_Create(zzMatrix1003ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;

    *ppRet = (zzMatrix1003ST *)AllocAndZeroMem(sizeof(zzMatrix1003ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);
    (*ppRet)->base.matrix_id     = MATRIX1003_ID;

    (*ppRet)->frame_num      = NOT_INIT_VALUE;
    (*ppRet)->frame_idx      = 0;

    (*ppRet)->demo_flag      = FALSE;

END:
    return sts;
}

zzStatus ZZMatrix1003_Release(zzMatrix1003ST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    ffmpeg_decode_uninit();

    return sts;
}


zzStatus ZZMatrix1003_Init(zzMatrix1003ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf, zz_char *pFilename)
{
    zzStatus  sts = ZZ_ERR_NONE;


    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZMatrix1003_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1003_ParseInputString error\n");
        goto END;
    }

    pSelf->base.pipe_ctrl = pPipeCtrl;
    pSelf->dst_surf       = pSurf;

    sts = ffmpeg_decode_init();
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ffmpeg_decode_init error\n");
        goto END;
    }

    sts = ffmpeg_input_file(pFilename);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF(" ffmpeg_input_file error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix1003_Start(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix1003ST *pSelf = GET_MATRIX1003(pMatrixBase);


    ZZDEBUG("Matrix %d Start\n", pSelf->base.matrix_id);

    //Get next frame
    sts = ffmpeg_next_frame(pSelf->dst_surf->ff_frame);
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

    pSelf->frame_idx++;

END:
    return sts;
}

zzStatus ZZMatrix1003_PartStart(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix1003ST *pSelf = GET_MATRIX1003(pMatrixBase);

    ZZDEBUG("Matrix %d PartStart\n", pSelf->base.matrix_id);

    if (pSelf->frame_num == pSelf->frame_idx &&
        FALSE == pSelf->demo_flag)
    {
        pMatrixBase->next_event = ZZ_EVENT_END;
        pSelf->base.pipe_ctrl->pipe_event = ZZ_EVENT_PIPE_EXIT;

        ZZPRINTF("Frame Number: %d\n", pSelf->frame_idx);
        goto END;
    }

    sts = ffmpeg_next_frame(pSelf->dst_surf->ff_frame);
    switch (sts)
    {
    case ZZ_ERR_NONE:
        pMatrixBase->next_event = ZZ_EVENT_PART_END;
        pSelf->frame_idx++;
        break;
    case ZZ_ERR_EOF_STREAM:
        pMatrixBase->next_event = ZZ_EVENT_END;
        pSelf->base.pipe_ctrl->pipe_event = ZZ_EVENT_PIPE_EXIT;
        sts = ZZ_ERR_NONE;

        ZZPRINTF("Frame Number: %d\n", pSelf->frame_idx);
        break;
    default:
        ZZPRINTF("ZZ_GetNextInputFrame error\n");
        goto END;
    }

    sts = ZZSurface_LockFrame(pSelf->dst_surf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_LockFrame error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix1003_ParseInputString(zzMatrix1003ST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;
    zzU8               i       = 1;

    for (i = 1; i < nArgNum; i++ )
    {
        CHECK_POINTER(strInput[i], ZZ_ERR_NULL_PTR);
        {
            if (0 == zz_strcmp(strInput[i], ZZ_STRING("-n")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &pSelf->frame_num);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-demo")))
            {
                ZZPRINTF("Running with demo mode, press Ctrl + c for exist\n");
                pSelf->demo_flag = TRUE;
            }

        }
    }

    return sts;
}
