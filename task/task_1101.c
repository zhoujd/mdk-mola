/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)task_1101.c
 */

#include "task_1101.h"
#include "app_defs.h"

//static functions
static zzStatus ZZTask1101_Init(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);
static zzStatus ZZTask1101_ExecInit(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask1101_Release(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask1101_Help(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);

static zzStatus ZZTask1101_CreateSurface(zzTask1101ST  *pSelf);
static zzStatus ZZTask1101_ReleaseSurface(zzTask1101ST  *pSelf);

zzStatus ZZTask1101_Register()
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzAppST       *pApp  = GetAPP();
    zzTask1101ST  *pSelf = NULL;

    pSelf = (zzTask1101ST *)AllocAndZeroMem(sizeof(zzTask1101ST));
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
                                ZZTask1101_Init,
                                ZZTask1101_ExecInit,
                                ZZTask1101_Release,
                                ZZTask1101_Help);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskFlowBase_DefaultFP  error\n");
        goto END;
    }
    
    pSelf->base.task_id = TASK1101_ID;
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

zzStatus ZZTask1101_Help(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    
    int                    i  = 0;
    const static zz_char*  help_doc[] = {
        ZZ_STRING("Task 1101 help welcome"),
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


zzStatus ZZTask1101_Init(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask1101ST  *pSelf = GET_TASK1101(pTaskBase);

    //use default context
    sts  = ZZTaskBase_Init(pTaskBase, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_Init  error\n");
        goto END;
    }

    sts = ZZVAContext_Create(&pSelf->ctx);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("VAContext_Create  error\n");
        goto END;
    }

    pSelf->surface[TASK1101_SCALING_SRC].context = &pSelf->ctx;
    pSelf->surface[TASK1101_SCALING_DST].context = &pSelf->ctx;
    sts = ZZTask1101_CreateSurface(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1101_CreateSurface  error\n");
        goto END;
    }
    
    //create martix 1101
    sts = ZZMatrix1101_Create(&pSelf->pMatrix1101);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1101_Create  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(pTaskBase, &pSelf->pMatrix1101->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }
    

END:
    return sts;
    
}

zzStatus ZZTask1101_ExecInit(zzTaskBaseST *pTaskBase)
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

zzStatus ZZTask1101_Release(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask1101ST  *pSelf = GET_TASK1101(pTaskBase);

    sts = ZZTask1101_ReleaseSurface(pSelf);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1101_ReleseSurface  error\n");
        goto END;
    }

    
    sts = ZZVAContext_Release(&pSelf->ctx);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZVAContext_Release  error\n");
        goto END;
    }
    
    //release martix 1101
    sts = ZZMatrix1101_Release(pSelf->pMatrix1101);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1101_Release  error\n");
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


zzStatus ZZTask1101_CreateSurface(zzTask1101ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    
    pSelf->surface[TASK1101_SCALING_SRC].frameInfo.Width      = 720;
    pSelf->surface[TASK1101_SCALING_SRC].frameInfo.Height     = 576;
    pSelf->surface[TASK1101_SCALING_SRC].frameInfo.FourCC     = ZZ_FOURCC_NV12;

    pSelf->surface[TASK1101_SCALING_SRC].frameInfo.CropX      = 0;
    pSelf->surface[TASK1101_SCALING_SRC].frameInfo.CropY      = 0;
    pSelf->surface[TASK1101_SCALING_SRC].frameInfo.CropW      = 720;
    pSelf->surface[TASK1101_SCALING_SRC].frameInfo.CropH      = 576;
    pSelf->surface[TASK1101_SCALING_SRC].frameInfo.PicStruct  = ZZ_PICSTRUCT_PROGRESSIVE;

    pSelf->surface[TASK1101_SCALING_DST].frameInfo.Width      = 1440;
    pSelf->surface[TASK1101_SCALING_DST].frameInfo.Height     = 1152;
    pSelf->surface[TASK1101_SCALING_DST].frameInfo.FourCC     = ZZ_FOURCC_NV12;

    pSelf->surface[TASK1101_SCALING_DST].frameInfo.CropX      = 0;
    pSelf->surface[TASK1101_SCALING_DST].frameInfo.CropY      = 0;
    pSelf->surface[TASK1101_SCALING_DST].frameInfo.CropW      = 1440;
    pSelf->surface[TASK1101_SCALING_DST].frameInfo.CropH      = 1152;
    pSelf->surface[TASK1101_SCALING_DST].frameInfo.PicStruct  = ZZ_PICSTRUCT_PROGRESSIVE;

    ZZPRINTF("Task1101 scc frameInfo\n");
    sts = ZZ_DumpFrameInfo(&pSelf->surface[TASK1101_SCALING_SRC].frameInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_DumpFrameInfo  error\n");
        goto END;
    }
    
    ZZPRINTF("Task1101 dcc frameInfo\n");
    sts = ZZ_DumpFrameInfo(&pSelf->surface[TASK1101_SCALING_DST].frameInfo);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_DumpFrameInfo  error\n");
        goto END;
    }

    sts = ZZSurface_Create(&pSelf->surface[TASK1101_SCALING_SRC]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_CreateEx  error\n");
        goto END;
    }

    sts = ZZSurface_Create(&pSelf->surface[TASK1101_SCALING_DST]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_CreateEx  error\n");
        goto END;
    }

    
END:
    return sts;

}

zzStatus ZZTask1101_ReleaseSurface(zzTask1101ST  *pSelf)
{
    zzStatus       sts    = ZZ_ERR_NONE;
    
    sts = ZZSurface_Release(&pSelf->surface[TASK1101_SCALING_SRC]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_Release  error\n");
        goto END;
    }
        
    sts = ZZSurface_Release(&pSelf->surface[TASK1101_SCALING_DST]);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZSurface_Release  error\n");
        goto END;
    }
	
END:
    return sts;
}
