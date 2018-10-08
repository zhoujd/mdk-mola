/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)matrix_9001.c
 */

#include "app_base.h"
#include "matrix_9001.h"

#define MATRIX9002_TITLE_MAX_LEN  256

static zzStatus ZZMatrix9001_Start(zzMatrixBaseST *pSelf);
static zzStatus ZZMatrix9001_PartStart(zzMatrixBaseST *pSelf);

//Matrix Cells
static zzMatrixCellST matrix_cells[] =
{
    {ZZ_EVENT_START,         NULL,  ZZMatrix9001_Start,      NULL},
    {ZZ_EVENT_PART_START,    NULL,  ZZMatrix9001_PartStart,  NULL},
};


zzStatus ZZMatrix9001_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    ZZPRINTF("Matrix 9001 : output frame to file\n");

    return sts;
}

zzStatus ZZMatrix9001_HelpRegister()
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

    pHelpInfo->help_id         = MATRIX9001_ID;
    pHelpInfo->desc_info       = ZZ_STRING("input one frame by one frame");
    pHelpInfo->pfnMatrixHelpFn = ZZMatrix9001_Help;

    sts =  ZZHelp_MatrixHelpAdd(&pApp->help, pHelpInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_MatrixHelpAdd  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix9001_Create(zzMatrix9001ST **ppRet)
{
    zzStatus  sts = ZZ_ERR_NONE;

    *ppRet = (zzMatrix9001ST *)AllocAndZeroMem(sizeof(zzMatrix9001ST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    (*ppRet)->base.cell_list = matrix_cells;
    (*ppRet)->base.cell_num  = ARRAY_NUM(matrix_cells);

    (*ppRet)->base.matrix_id = MATRIX9001_ID;


END:
    return sts;
}


zzStatus ZZMatrix9001_Init(zzMatrix9001ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST   *pPipeCtrl,
                           zzSurfaceST    *pSurf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    pSelf->base.pipe_ctrl = pPipeCtrl;
    pSelf->src_surf       = *pSurf;

    pSelf->srcW = pSelf->src_surf.frameInfo.Width;
    pSelf->srcH = pSelf->src_surf.frameInfo.Height;
    pSelf->dstW = pSelf->src_surf.frameInfo.Width;
    pSelf->dstH = pSelf->src_surf.frameInfo.Height;

#if ZZ_LIBVA_USE_X11
    static zz_char title[MATRIX9002_TITLE_MAX_LEN] = {0};
    sprintf(title, "zzmediaxapp %dx%d", pSelf->src_surf.frameInfo.Width, pSelf->src_surf.frameInfo.Height);

    const char* currentDisplay = getenv("DISPLAY");
    if (currentDisplay)
    {
        pSelf->mDisplay = XOpenDisplay(currentDisplay);
    }
    else
    {
        pSelf->mDisplay = XOpenDisplay(VAAPI_X_DEFAULT_DISPLAY);
    }

    if (pSelf->mDisplay == NULL)
    {
        ZZPRINTF("open_display error\n");
        sts = ZZ_ERR_UNKNOWN;
        goto END;
    }

    pSelf->mScreen = DefaultScreen(pSelf->mDisplay);  // screen ID
    pSelf->mRootWin = RootWindow(pSelf->mDisplay, pSelf->mScreen);   // root window ID

    //create window to display
    pSelf->mShowWin = XCreateSimpleWindow(pSelf->mDisplay,
                                          pSelf->mRootWin,
                                          0,
                                          0,
                                          pSelf->dstW,
                                          pSelf->dstH,
                                          1,
                                          BlackPixel(pSelf->mDisplay, pSelf->mScreen),
                                          WhitePixel(pSelf->mDisplay, pSelf->mScreen));

    if (pSelf->mShowWin)
    {
        XSizeHints sizehints;
        sizehints.x      = 0;
        sizehints.y      = 0;
        sizehints.width  = pSelf->dstW;
        sizehints.height = pSelf->dstH;
        sizehints.flags  = USSize | USPosition;
        XSetNormalHints(pSelf->mDisplay, pSelf->mShowWin, &sizehints);
        XSetStandardProperties(pSelf->mDisplay,
                               pSelf->mShowWin,
                               title,
                               title,
                               None,
                               (char **)NULL,
                               0,
                               &sizehints);
        XMapWindow(pSelf->mDisplay, pSelf->mShowWin);
    }

    pSelf->mGC = XCreateGC(pSelf->mDisplay, pSelf->mShowWin, 0, &pSelf->mGS);
    XSelectInput(pSelf->mDisplay, pSelf->mShowWin, KeyPressMask | StructureNotifyMask);
    XSync(pSelf->mDisplay, False);

END:
#endif //ZZ_LIBVA_USE_X11

    return sts;
}


zzStatus ZZMatrix9001_Release(zzMatrix9001ST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

#if ZZ_LIBVA_USE_X11
    XCloseDisplay(pSelf->mDisplay);
#endif //ZZ_LIBVA_USE_X11

    return sts;
}

zzStatus ZZMatrix9001_Start(zzMatrixBaseST *pMatrixBase)
{
    zzStatus        sts   = ZZ_ERR_NONE;
    zzMatrix9001ST *pSelf = GET_MATRIX9001(pMatrixBase);

    ZZDEBUG("Matrix 9001 Start\n");

    if (pSelf->base.pipe_ctrl->pipe_event == ZZ_EVENT_PIPE_EXIT)
    {
        ZZDEBUG("(%s) get exist event\n", __FUNCTION__);
        pMatrixBase->next_event = ZZ_EVENT_END;
        goto END;
    }

    sts = ZZMatrix9001_ProcNextFrame(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9001_ProcNextFrame  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix9001_PartStart(zzMatrixBaseST *pMatrixBase)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    zzMatrix9001ST *pSelf = GET_MATRIX9001(pMatrixBase);

    ZZDEBUG("Matrix 9001 PartStart\n");

    if (pSelf->base.pipe_ctrl->pipe_event == ZZ_EVENT_PIPE_EXIT)
    {
        ZZDEBUG("(%s) get exist event\n", __FUNCTION__);
        pMatrixBase->next_event = ZZ_EVENT_END;
        goto END;
    }

    sts = ZZMatrix9001_ProcNextFrame(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9001_ProcNextFrame  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMatrix9001_ProcNextFrame(zzMatrix9001ST *pSelf)
{
    zzStatus  sts    = ZZ_ERR_NONE;
    VAStatus    va_res = VA_STATUS_SUCCESS;

    CHECK_POINTER(pSelf, ZZ_ERR_NOT_INITIALIZED);

#if ZZ_LIBVA_USE_X11
    va_res = vaPutSurface(pSelf->src_surf.context->va_dpy,
                          pSelf->src_surf.id,
                          pSelf->mShowWin,
                          pSelf->srcX,
                          pSelf->srcY,
                          pSelf->srcW,
                          pSelf->srcH,
                          pSelf->dstX,
                          pSelf->dstY,
                          pSelf->dstW,
                          pSelf->dstH,
                          NULL,
                          0,
                          pSelf->flags);
#endif //ZZ_LIBVA_USE_X11
    sts = va_to_zz_status(va_res);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("vaPutSurface error\n");
        goto END;
    }

END:
    return sts;
}
