/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)task_2002.c
 */

#include "task_2002.h"
#include "app_defs.h"

//static functions
static zzStatus ZZTask2002_Init(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);
static zzStatus ZZTask2002_ExecInit(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask2002_PreExec(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);
static zzStatus ZZTask2002_Exec(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask2002_PostExec(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask2002_Release(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask2002_Help(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);

static zzStatus ZZTask2002_ParseInputString(zzTask2002ST  *pSelf, int nArgNum, char **strInput);
static zzStatus ZZTask2002_ParseOutputInfo(zzTask2002ST  *pSelf, int nArgNum, char **strInput);
static zzStatus ZZTask2002_DefaultParam(zzTask2002ST  *pSelf);

static zzStatus ZZTask2002_InitMatrix(zzTask2002ST  *pSelf, zzU16 argc, zz_char **argv);
static zzStatus ZZTask2002_ReleaseMatrix(zzTask2002ST  *pSelf);

static zzStatus ZZTask2002_CreateSurface(zzTask2002ST  *pSelf);
static zzStatus ZZTask2002_ReleaseSurface(zzTask2002ST  *pSelf);


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


zzStatus ZZTask2002_Register()
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzAppST       *pApp  = GetAPP();
    zzTask2002ST  *pSelf = NULL;

    pSelf = (zzTask2002ST *)AllocAndZeroMem(sizeof(zzTask2002ST));
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
                                ZZTask2002_Init,
                                ZZTask2002_ExecInit,
                                ZZTask2002_PreExec,
                                ZZTask2002_Exec,
                                ZZTask2002_PostExec,
                                ZZTask2002_Release,
                                ZZTask2002_Help);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskFlowBase_DefaultFP  error\n");
        goto END;
    }

    pSelf->base.task_id = TASK2002_ID;
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

zzStatus ZZTask2002_Help(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;

    int                    i  = 0;
    const static zz_char*  help_doc[] = {
        ZZ_STRING("Task 2002 help welcome"),
        ZZ_STRING("Task 2002 = Matrix1002 + Matrix2002 + Matrix9001"),
        ZZ_STRING("Task 2002 example:"),
        ZZ_STRING("-task 2002 -sw 352 -sh 288 -dw 720 -dh 576 -scc nv12 -dcc nv12 -i input.nv12 -o output.nv12 -angle 1"),
        ZZ_STRING("-task 2002 -sw 352 -sh 288 -dw 352 -dh 288 -scc nv12 -dcc nv12 -spic 0 -dpic 1  -i input.nv12 -o output.nv12 --deinterlace algo=bob"),
        ZZ_STRING("-task 2002 -sw 720 -sh 576 -dw 1440 -dh 1152 -scc nv12 -dcc nv12 -i input.nv12 -o output.nv12 --sharpness factor=64"),
        ZZ_STRING("-task 2002 -sw 720 -sh 576 -dw 1440 -dh 1152 -scc nv12 -dcc nv12 -i input.nv12 -o output.nv12 --amp hue=20:contrast=2:brightness=50:saturation=2"),
        ZZ_STRING("-task 2002 -sw 720 -sh 480 -dw 720 -dh 480 -scc nv12 -dcc nv12 -i input.nv12 -o output.nv12 --denoise factor=64"),
        ZZ_STRING("-task 2002 -sw 720 -sh 576 -dw 1440 -dh 1152 -scc nv12 -dcc argb -i input.nv12 -o output.nv12 --comp alpha=0.5:row=1:col=2"),
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


zzStatus ZZTask2002_Init(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask2002ST  *pSelf = GET_TASK2002(pTaskBase);

    pSelf->surface[TASK2002_SCALING_SRC].id = VA_INVALID_ID;
    pSelf->surface[TASK2002_SCALING_DST].id = VA_INVALID_ID;
    pSelf->ctx.id = VA_INVALID_ID;

    //set default param
    sts = ZZTask2002_DefaultParam(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2002_DefaultParam  error\n");
        goto END;
    }

    //use default context
    sts = ZZTaskBase_Init(pTaskBase, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_Init  error\n");
        goto END;
    }

    sts = ZZTask2002_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2002_ParseInputString  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2002_ExecInit(zzTaskBaseST *pTaskBase)
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

zzStatus ZZTask2002_PreExec(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask2002ST  *pSelf = GET_TASK2002(pTaskBase);

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

    pSelf->surface[TASK2002_SCALING_SRC].context = &pSelf->ctx;
    pSelf->surface[TASK2002_SCALING_DST].context = &pSelf->ctx;
    sts = ZZTask2002_CreateSurface(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2002_CreateSurface  error\n");
        goto END;
    }


    sts = ZZTask2002_InitMatrix(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2002_InitMatrix error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2002_Exec(zzTaskBaseST *pTaskBase)
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

zzStatus ZZTask2002_PostExec(zzTaskBaseST *pTaskBase)
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

zzStatus ZZTask2002_Release(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask2002ST  *pSelf = GET_TASK2002(pTaskBase);

    ZZDEBUG("ZZTask2002_Release\n");

    sts = ZZTask2002_ReleaseSurface(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2002_ReleseSurface  error\n");
        goto END;
    }


    sts = ZZVAContext_Release(&pSelf->ctx);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZVAContext_Release  error\n");
        goto END;
    }

    //release martix 1002
    sts = ZZTask2002_ReleaseMatrix(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2002_Release  error\n");
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

zzStatus ZZTask2002_ReleaseMatrix(zzTask2002ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    //release martix 1002
    if (NULL != pSelf->pMatrix1002)
    {
        sts = ZZMatrix1002_Release(pSelf->pMatrix1002);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZMatrix2002_Release 1002 error\n");
            goto END;
        }
    }

    //release martix 2002
    if (NULL != pSelf->pMatrix2002)
    {
        sts = ZZMatrix2002_Release(pSelf->pMatrix2002);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZMatrix2002_Release  2002 error\n");
            goto END;
        }
    }

    //release martix 9001
    if (NULL != pSelf->pMatrix9001)
    {
        sts = ZZMatrix9001_Release(pSelf->pMatrix9001);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZMatrix2002_Release  9001 error\n");
            goto END;
        }
    }

END:
    return sts;
}

zzStatus ZZTask2002_CreateSurface(zzTask2002ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    sts = ZZ_ParamInfo2SurfInfo(&pSelf->surface[TASK2002_SCALING_SRC].frameInfo,
                                &pSelf->params.frameInfo[VPP_IN]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_ParamInfo2SurfInfo  error\n");
        goto END;
    }

    sts = ZZ_ParamInfo2SurfInfo(&pSelf->surface[TASK2002_SCALING_DST].frameInfo,
                                &pSelf->params.frameInfo[VPP_OUT]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_ParamInfo2SurfInfo  error\n");
        goto END;
    }

    ZZPRINTF("Task2002 scc frameInfo\n");
    sts = ZZ_DumpFrameInfo(&pSelf->surface[TASK2002_SCALING_SRC].frameInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_DumpFrameInfo  error\n");
        goto END;
    }

    ZZPRINTF("Task2002 dcc frameInfo\n");
    sts = ZZ_DumpFrameInfo(&pSelf->surface[TASK2002_SCALING_DST].frameInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_DumpFrameInfo  error\n");
        goto END;
    }

    sts = ZZSurface_Create(&pSelf->surface[TASK2002_SCALING_SRC]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_CreateEx  error\n");
        goto END;
    }

    sts = ZZSurface_Create(&pSelf->surface[TASK2002_SCALING_DST]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_CreateEx  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask2002_ReleaseSurface(zzTask2002ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    sts = ZZSurface_Release(&pSelf->surface[TASK2002_SCALING_SRC]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_Release  error\n");
        goto END;
    }

    sts = ZZSurface_Release(&pSelf->surface[TASK2002_SCALING_DST]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_Release  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZTask2002_InitMatrix(zzTask2002ST  *pSelf, zzU16 argc, zz_char **argv)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    //create martix 1002
    sts = ZZMatrix1002_Create(&pSelf->pMatrix1002);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1002_Create  error\n");
        goto END;
    }

    //init martix 1002
    sts = ZZMatrix1002_Init(pSelf->pMatrix1002, argc, argv,
                            &pSelf->base.pipe_ctrl,
                            &pSelf->surface[TASK2002_SCALING_SRC], pSelf->params.strSrcFile);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1002_Init  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(&pSelf->base, &pSelf->pMatrix1002->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }

    //create martix 2002
    sts = ZZMatrix2002_Create(&pSelf->pMatrix2002);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2002_Create  error\n");
        goto END;
    }

    //init martix 2002
    sts = ZZMatrix2002_Init(pSelf->pMatrix2002, argc, argv,
                            &pSelf->base.pipe_ctrl,
                            &pSelf->ctx,
                            &pSelf->surface[TASK2002_SCALING_SRC],
                            &pSelf->surface[TASK2002_SCALING_DST]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2002_Init  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(&pSelf->base, &pSelf->pMatrix2002->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }
    //create martix 9001
    sts = ZZMatrix9001_Create(&pSelf->pMatrix9001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9001_Create  error\n");
        goto END;
    }

    //init martix 9001
    sts = ZZMatrix9001_Init(pSelf->pMatrix9001, argc, argv,
                            &pSelf->base.pipe_ctrl,
                            &pSelf->surface[TASK2002_SCALING_DST]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9001_Init  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(&pSelf->base, &pSelf->pMatrix9001->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }

END:
    return sts;
}


zzStatus ZZTask2002_ParseOutputInfo(zzTask2002ST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus                 sts      = ZZ_ERR_NONE;
    zzU8                     i        = 1;
    zzTask2002InputParamsST  *pParams = &pSelf->params;

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

zzStatus ZZTask2002_DefaultParam(zzTask2002ST  *pSelf)
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

zzStatus ZZTask2002_ParseInputString(zzTask2002ST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;

    sts = ZZ_ParseOwnFrameInfo(pSelf->params.frameInfo, nArgNum, strInput);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_ParseOwnFrameInfo  error\n");
        goto END;
    }

    sts = ZZTask2002_ParseOutputInfo(pSelf, nArgNum, strInput);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask2002_ParseOutputInfo  error\n");
        goto END;
    }

END:
    return sts;
}
