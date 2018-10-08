/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)task_1001.c
 */

#include "task_1001.h"
#include "app_defs.h"

//static functions
static zzStatus ZZTask1001_Init(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);
static zzStatus ZZTask1001_ExecInit(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask1001_PreExec(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask1001_PostExec(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask1001_Release(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask1001_Help(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);

static zzStatus ZZTask1001_ParseInputString(zzTask1001ST  *pSelf, int nArgNum, char **strInput);
static zzStatus ZZTask1001_ParseOutputInfo(zzTask1001ST  *pSelf, int nArgNum, char **strInput);
static zzStatus ZZTask1001_DefaultParam(zzTask1001ST  *pSelf);

static zzStatus ZZTask1001_InitMatrix(zzTask1001ST  *pSelf, zzU16 argc, zz_char **argv);
static zzStatus ZZTask1001_ReleaseMatrix(zzTask1001ST  *pSelf);

static zzStatus ZZTask1001_CreateSurface(zzTask1001ST  *pSelf);
static zzStatus ZZTask1001_ReleaseSurface(zzTask1001ST  *pSelf);


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


zzStatus ZZTask1001_Register()
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzAppST       *pApp  = GetAPP();
    zzTask1001ST  *pSelf = NULL;

    pSelf = (zzTask1001ST *)AllocAndZeroMem(sizeof(zzTask1001ST));
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
                                ZZTask1001_Init,
                                ZZTask1001_ExecInit,
                                ZZTask1001_PreExec,
                                ZZTask1001_PostExec,
                                ZZTask1001_Release,
                                ZZTask1001_Help);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskFlowBase_DefaultFP  error\n");
        goto END;
    }

    pSelf->base.task_id = TASK1001_ID;
    pSelf->base.desc_info = ZZ_STRING("This is only for develop\n");

    pSelf->surface[TASK1001_SCALING_SRC].id = VA_INVALID_ID;
    pSelf->ctx.id = VA_INVALID_ID;

    sts =  ZZApp_AddFlow(pApp, &pSelf->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_AddFlow  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZTask1001_Help(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;

    int                    i  = 0;
    const static zz_char*  help_doc[] = {
        ZZ_STRING("Task 1001 help welcome"),
        ZZ_STRING("Task 1001 = Matrix1002 + Matrix9002"),
        ZZ_STRING("Task 1001 example:"),
        ZZ_STRING("-task 1001 -sw 352 -sh 288 -dw 720 -dh 576 -scc nv12 -dcc nv12 -i input.nv12 -o output.nv12 -n 10"),
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


zzStatus ZZTask1001_Init(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask1001ST  *pSelf = GET_TASK1001(pTaskBase);

    //set default param
    sts = ZZTask1001_DefaultParam(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1001_DefaultParam  error\n");
        goto END;
    }

    //use default context
    sts = ZZTaskBase_Init(pTaskBase, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_Init  error\n");
        goto END;
    }

    sts = ZZTask1001_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1001_ParseInputString  error\n");
        goto END;
    }

    sts = ZZVAContext_Create(&pSelf->ctx);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("VAContext_Create  error\n");
        goto END;
    }

    pSelf->surface[TASK1001_SCALING_SRC].context = &pSelf->ctx;
    sts = ZZTask1001_CreateSurface(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1001_CreateSurface  error\n");
        goto END;
    }


    sts = ZZTask1001_InitMatrix(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1001_InitMatrix error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask1001_ExecInit(zzTaskBaseST *pTaskBase)
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

zzStatus ZZTask1001_PreExec(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;

    sts  = ZZTaskBase_PreExec(pTaskBase);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_PreExec  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask1001_PostExec(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;

    sts  = ZZTaskBase_PostExec(pTaskBase);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_PostInit  error\n");
        goto END;
    }

END:
    return sts;

}


zzStatus ZZTask1001_Release(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask1001ST  *pSelf = GET_TASK1001(pTaskBase);

    ZZDEBUG("ZZTask1001_Release\n");

    sts = ZZTask1001_ReleaseSurface(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1001_ReleseSurface  error\n");
        goto END;
    }


    sts = ZZVAContext_Release(&pSelf->ctx);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZVAContext_Release  error\n");
        goto END;
    }

    //release martix 1002
    sts = ZZTask1001_ReleaseMatrix(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1001_Release  error\n");
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

zzStatus ZZTask1001_ReleaseMatrix(zzTask1001ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    //release martix 1002
    if (NULL != pSelf->pMatrix1002)
    {
        sts = ZZMatrix1002_Release(pSelf->pMatrix1002);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZMatrix1001_Release 1002 error\n");
            goto END;
        }
    }

    //release martix 9002
    if (NULL != pSelf->pMatrix9002)
    {
        sts = ZZMatrix9002_Release(pSelf->pMatrix9002);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZMatrix1001_Release  9002 error\n");
            goto END;
        }
    }

END:
    return sts;
}

zzStatus ZZTask1001_CreateSurface(zzTask1001ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    sts = ZZ_ParamInfo2SurfInfo(&pSelf->surface[TASK1001_SCALING_SRC].frameInfo,
                                &pSelf->params.frameInfo[VPP_IN]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_ParamInfo2SurfInfo  error\n");
        goto END;
    }

    ZZPRINTF("Task1001 scc frameInfo\n");
    sts = ZZ_DumpFrameInfo(&pSelf->surface[TASK1001_SCALING_SRC].frameInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_DumpFrameInfo  error\n");
        goto END;
    }

    sts = ZZSurface_Create(&pSelf->surface[TASK1001_SCALING_SRC]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_CreateEx  error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZTask1001_ReleaseSurface(zzTask1001ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;

    sts = ZZSurface_Release(&pSelf->surface[TASK1001_SCALING_SRC]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_Release  error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZTask1001_InitMatrix(zzTask1001ST  *pSelf, zzU16 argc, zz_char **argv)
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
                            &pSelf->surface[TASK1001_SCALING_SRC], pSelf->params.strSrcFile);
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

    //create martix 9002
    sts = ZZMatrix9002_Create(&pSelf->pMatrix9002);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9002_Create  error\n");
        goto END;
    }

    //init martix 9002
    sts = ZZMatrix9002_Init(pSelf->pMatrix9002, argc, argv,
                            &pSelf->base.pipe_ctrl,
                            &pSelf->surface[TASK1001_SCALING_SRC], pSelf->params.strDstFile);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9002_Init  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(&pSelf->base, &pSelf->pMatrix9002->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }

END:
    return sts;
}


zzStatus ZZTask1001_ParseOutputInfo(zzTask1001ST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus                 sts      = ZZ_ERR_NONE;
    zzU8                     i        = 1;
    zzTask1001InputParamsST  *pParams = &pSelf->params;

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
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-o")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_strcpy(pParams->strDstFile, strInput[i]);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-n")) )
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &pParams->numFrames);
            }

            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-screen")) )
            {
                i++;
                pSelf->screen_flag = TRUE;
            }
        }
    }

    return sts;
}

zzStatus ZZTask1001_DefaultParam(zzTask1001ST  *pSelf)
{
    zzStatus sts = ZZ_ERR_NONE;
    zzU8        i   = 0;

    for (i = 0; i < VPP_COUNT; i++)
    {
        pSelf->params.frameInfo[i] = taskParam[i];
    }

    //file name
    zz_strncpy(pSelf->params.strSrcFile, ZZ_STRING(""), MAX_FILENAME_LEN - 1);
    zz_strncpy(pSelf->params.strDstFile, ZZ_STRING(""), MAX_FILENAME_LEN - 1);

    return sts;
}

zzStatus ZZTask1001_ParseInputString(zzTask1001ST  *pSelf, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;

    sts = ZZ_ParseOwnFrameInfo(pSelf->params.frameInfo, nArgNum, strInput);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_ParseOwnFrameInfo  error\n");
        goto END;
    }

    sts = ZZTask1001_ParseOutputInfo(pSelf, nArgNum, strInput);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1001_ParseOutputInfo  error\n");
        goto END;
    }

END:
    return sts;
}
