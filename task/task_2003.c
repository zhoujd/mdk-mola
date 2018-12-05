/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)task_2003.c
 */

#include "task_2003.h"
#include "app_defs.h"

//static functions
static zzStatus ZZTask2003_Init(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);
static zzStatus ZZTask2003_ExecInit(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask2003_PreExec(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);
static zzStatus ZZTask2003_Exec(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask2003_PostExec(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask2003_Release(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask2003_Help(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);

static zzStatus ZZTask2003_ParseInputString(zzTask2003ST  *pSelf, int nArgNum, char **strInput);
static zzStatus ZZTask2003_ParseOutputInfo(zzTask2003ST  *pSelf, int nArgNum, char **strInput);
static zzStatus ZZTask2003_DefaultParam(zzTask2003ST  *pSelf);

static zzStatus ZZTask2003_InitMatrix(zzTask2003ST  *pSelf, zzU16 argc, zz_char **argv);
static zzStatus ZZTask2003_ReleaseMatrix(zzTask2003ST  *pSelf);

static zzStatus ZZTask2003_CreateSurface(zzTask2003ST  *pSelf);
static zzStatus ZZTask2003_ReleaseSurface(zzTask2003ST  *pSelf);


//default parameter
const static zzOwnFrameInfoST  taskParam[VPP_COUNT] =
{
    {352, 0, 288, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, ZZ_FOURCC_NV12, ZZ_PICSTRUCT_UNKNOWN, 30},
    {352, 0, 288, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, ZZ_FOURCC_NV12, ZZ_PICSTRUCT_UNKNOWN, 30},
    {352, 0, 288, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, ZZ_FOURCC_NV12, ZZ_PICSTRUCT_UNKNOWN, 30},
    {352, 0, 288, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, ZZ_FOURCC_NV12, ZZ_PICSTRUCT_UNKNOWN, 30},
    {352, 0, 288, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, ZZ_FOURCC_NV12, ZZ_PICSTRUCT_UNKNOWN, 30},
    {352, 0, 288, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, ZZ_FOURCC_NV12, ZZ_PICSTRUCT_UNKNOWN, 30},
    {352, 0, 288, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, ZZ_FOURCC_NV12, ZZ_PICSTRUCT_UNKNOWN, 30},
    {352, 0, 288, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, ZZ_FOURCC_NV12, ZZ_PICSTRUCT_UNKNOWN, 30},
    {352, 0, 288, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, 0, 0, NOT_INIT_VALUE, NOT_INIT_VALUE, ZZ_FOURCC_NV12, ZZ_PICSTRUCT_UNKNOWN, 30},
};


zzStatus ZZTask2003_Register()
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzAppST       *pApp  = GetAPP();
    zzTask2003ST  *pSelf = NULL;

    pSelf = (zzTask2003ST *)AllocAndZeroMem(sizeof(zzTask2003ST));
    if (pSelf == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    sts = ZZTaskBase_DefaultFP((zzTaskBaseST *)pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_DefaultFP  error\n");
        goto END;
    }

    sts = ZZTaskBase_RegisterFP(&pSelf->base,
                                ZZTask2003_Init,
                                ZZTask2003_ExecInit,
                                ZZTask2003_PreExec,
                                ZZTask2003_Exec,
                                ZZTask2003_PostExec,
                                ZZTask2003_Release,
                                ZZTask2003_Help);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskFlowBase_DefaultFP  error\n");
        goto END;
    }

    pSelf->base.task_id = TASK2003_ID;
    pSelf->base.desc_info = ZZ_STRING("This is only for develop\n");

    sts =  ZZApp_AddFlow(pApp, &pSelf->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_AddFlow  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZTask2003_Help(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;

    int                    i  = 0;
    const static zz_char*  help_doc[] = {
        ZZ_STRING("Task 2003 help welcome"),
        ZZ_STRING("Task 2003 = Matrix1003 + Matrix2003 + Matrix9003"),
        ZZ_STRING("Task 2003 example:"),
        ZZ_STRING("-task 2003 -sw 352 -sh 288 -dw 720 -dh 576 -scc nv12 -dcc nv12 -i input.nv12 -o output.nv12 -angle 1"),
        ZZ_STRING("-task 2003 -sw 352 -sh 288 -dw 352 -dh 288 -scc nv12 -dcc nv12 -spic 0 -dpic 1  -i input.nv12 -o output.nv12 --deinterlace algo=bob"),
        ZZ_STRING("-task 2003 -sw 720 -sh 576 -dw 1440 -dh 1152 -scc nv12 -dcc nv12 -i input.nv12 -o output.nv12 --sharpness factor=64"),
        ZZ_STRING("-task 2003 -sw 720 -sh 576 -dw 1440 -dh 1152 -scc nv12 -dcc nv12 -i input.nv12 -o output.nv12 --amp hue=20:contrast=2:brightness=50:saturation=2"),
        ZZ_STRING("-task 2003 -sw 720 -sh 480 -dw 720 -dh 480 -scc nv12 -dcc nv12 -i input.nv12 -o output.nv12 --denoise factor=64"),
        ZZ_STRING("-task 2003 -sw 720 -sh 576 -dw 1440 -dh 1152 -scc nv12 -dcc argb -i input.nv12 -o output.nv12 --comp alpha=0.5:row=1:col=2"),
        NULL,
    };

    //use default context
    sts  = ZZTaskBase_Help(pTaskBase, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_Init  error\n");
        goto END;
    }

    for (i = 0; help_doc[i] != NULL; i++)
    {
        ZZPRINTF("%s\n", help_doc[i]);
    }

END:
    return sts;

}


zzStatus ZZTask2003_Init(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask2003ST  *pSelf = GET_TASK2003(pTaskBase);

    pSelf->surface[TASK2003_SCALING_SRC].id = VA_INVALID_ID;
    pSelf->surface[TASK2003_SCALING_DST].id = VA_INVALID_ID;
    pSelf->ctx.id = VA_INVALID_ID;

    //set default param
    sts = ZZTask2003_DefaultParam(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2003_DefaultParam  error\n");
        goto END;
    }

    //use default context
    sts = ZZTaskBase_Init(pTaskBase, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_Init  error\n");
        goto END;
    }

    sts = ZZTask2003_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2003_ParseInputString  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2003_ExecInit(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;

    sts  = ZZTaskBase_ExecInit(pTaskBase);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_ExecInit  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2003_PreExec(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask2003ST  *pSelf = GET_TASK2003(pTaskBase);

    sts  = ZZTaskBase_PreExec(pTaskBase, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_PreExec  error\n");
        goto END;
    }

    sts = ZZVAContext_Create(&pSelf->ctx);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("VAContext_Create  error\n");
        goto END;
    }

    pSelf->surface[TASK2003_SCALING_SRC].context = &pSelf->ctx;
    pSelf->surface[TASK2003_SCALING_DST].context = &pSelf->ctx;
    sts = ZZTask2003_CreateSurface(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2003_CreateSurface  error\n");
        goto END;
    }


    sts = ZZTask2003_InitMatrix(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2003_InitMatrix error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2003_Exec(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;

    sts  = ZZTaskBase_Exec(pTaskBase);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_Exec  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2003_PostExec(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;

    sts  = ZZTaskBase_PostExec(pTaskBase);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_PostExec  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2003_Release(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask2003ST  *pSelf = GET_TASK2003(pTaskBase);

    ZZDEBUG("ZZTask2003_Release\n");

    sts = ZZTask2003_ReleaseSurface(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2003_ReleseSurface  error\n");
        goto END;
    }


    sts = ZZVAContext_Release(&pSelf->ctx);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZVAContext_Release  error\n");
        goto END;
    }

    //release martix 1003
    sts = ZZTask2003_ReleaseMatrix(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2003_Release  error\n");
        goto END;
    }

    sts  = ZZTaskBase_Release(pTaskBase);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBaseExec_Init  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2003_ReleaseMatrix(zzTask2003ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    //release martix 1003
    if (NULL != pSelf->pMatrix1003)
    {
        sts = ZZMatrix1003_Release(pSelf->pMatrix1003);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZMatrix2003_Release 1003 error\n");
            goto END;
        }
    }

    //release martix 2002
    if (NULL != pSelf->pMatrix2002)
    {
        sts = ZZMatrix2002_Release(pSelf->pMatrix2002);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZMatrix2003_Release 2002 error\n");
            goto END;
        }
    }

    //release martix 9003
    if (NULL != pSelf->pMatrix9003)
    {
        sts = ZZMatrix9003_Release(pSelf->pMatrix9003);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZMatrix2003_Release 9003 error\n");
            goto END;
        }
    }

END:
    return sts;
}

zzStatus ZZTask2003_CreateSurface(zzTask2003ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    sts = ZZ_ParamInfo2SurfInfo(&pSelf->surface[TASK2003_SCALING_SRC].frameInfo,
                                &pSelf->params.frameInfo[VPP_IN]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_ParamInfo2SurfInfo  error\n");
        goto END;
    }

    sts = ZZ_ParamInfo2SurfInfo(&pSelf->surface[TASK2003_SCALING_DST].frameInfo,
                                &pSelf->params.frameInfo[VPP_OUT]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_ParamInfo2SurfInfo  error\n");
        goto END;
    }

    ZZPRINTF("Task2003 scc frameInfo\n");
    sts = ZZ_DumpFrameInfo(&pSelf->surface[TASK2003_SCALING_SRC].frameInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_DumpFrameInfo  error\n");
        goto END;
    }

    ZZPRINTF("Task2003 dcc frameInfo\n");
    sts = ZZ_DumpFrameInfo(&pSelf->surface[TASK2003_SCALING_DST].frameInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_DumpFrameInfo  error\n");
        goto END;
    }

    sts = ZZSurface_Create(&pSelf->surface[TASK2003_SCALING_SRC]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_CreateEx  error\n");
        goto END;
    }

    sts = ZZSurface_Create(&pSelf->surface[TASK2003_SCALING_DST]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_CreateEx  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2003_ReleaseSurface(zzTask2003ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    sts = ZZSurface_Release(&pSelf->surface[TASK2003_SCALING_SRC]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_Release  error\n");
        goto END;
    }

    sts = ZZSurface_Release(&pSelf->surface[TASK2003_SCALING_DST]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_Release  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZTask2003_InitMatrix(zzTask2003ST  *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    //create martix 1003
    sts = ZZMatrix1003_Create(&pSelf->pMatrix1003);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1003_Create  error\n");
        goto END;
    }

    //init martix 1003
    sts = ZZMatrix1003_Init(pSelf->pMatrix1003, argc, argv,
                            &pSelf->base.pipe_ctrl,
                            &pSelf->surface[TASK2003_SCALING_SRC], pSelf->params.strSrcFile);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1003_Init  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(&pSelf->base, &pSelf->pMatrix1003->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }

    //create martix 2002
    sts = ZZMatrix2002_Create(&pSelf->pMatrix2002);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2003_Create  error\n");
        goto END;
    }

    //init martix 2002
    sts = ZZMatrix2002_Init(pSelf->pMatrix2002, argc, argv,
                            &pSelf->base.pipe_ctrl,
                            &pSelf->ctx,
                            &pSelf->surface[TASK2003_SCALING_SRC],
                            &pSelf->surface[TASK2003_SCALING_DST]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2003_Init  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(&pSelf->base, &pSelf->pMatrix2002->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }
    //create martix 9003
    sts = ZZMatrix9003_Create(&pSelf->pMatrix9003);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9003_Create  error\n");
        goto END;
    }

    //init martix 9003
    sts = ZZMatrix9003_Init(pSelf->pMatrix9003, argc, argv,
                            &pSelf->base.pipe_ctrl,
                            &pSelf->surface[TASK2003_SCALING_DST],
                            "dump.yuv");
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9003_Init  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(&pSelf->base, &pSelf->pMatrix9003->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }

END:
    return sts;
}


zzStatus ZZTask2003_ParseOutputInfo(zzTask2003ST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus                 sts      = ZZ_ERR_NONE;
    zzU8                     i        = 1;
    zzTask2003InputParamsST  *pParams = &pSelf->params;

    for (i = 1; i < nArgNum; i++ )
    {
        CHECK_POINTER(strInput[i], ZZ_ERR_NULL_PTR);
        {
            if (0 == zz_strcmp(strInput[i], ZZ_STRING("-i")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_strcpy(pParams->strSrcFile, strInput[i]);
            }
        }
    }

    return sts;
}

zzStatus ZZTask2003_DefaultParam(zzTask2003ST  *pSelf)
{
    zzStatus sts = ZZ_ERR_NONE;
    zzU8        i   = 0;

    for (i = 0; i < VPP_COUNT; i++)
    {
        pSelf->params.frameInfo[i] = taskParam[i];
    }

    //file name
    zz_strncpy(pSelf->params.strSrcFile, ZZ_STRING(""), MAX_FILENAME_LEN - 1);

    return sts;
}

zzStatus ZZTask2003_ParseInputString(zzTask2003ST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;

    sts = ZZ_ParseOwnFrameInfo(pSelf->params.frameInfo, nArgNum, strInput);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_ParseOwnFrameInfo  error\n");
        goto END;
    }

    sts = ZZTask2003_ParseOutputInfo(pSelf, nArgNum, strInput);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2003_ParseOutputInfo  error\n");
        goto END;
    }

END:
    return sts;
}
