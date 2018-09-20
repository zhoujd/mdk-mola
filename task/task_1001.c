/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)task_1001.c
 */

#include "task_1001.h"
#include "app_defs.h"

//static functions
static zzStatus ZZTask1001_Init(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);
static zzStatus ZZTask1001_ExecInit(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask1001_Release(zzTaskBaseST *pTaskBase);
static zzStatus ZZTask1001_Help(zzTaskBaseST *pTaskBase, zzU16 argc, zz_char **argv);

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
                                    ZZTask1001_Release,
                                    ZZTask1001_Help);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_DefaultFP  error\n");
        goto END;
    }
    
    pSelf->base.task_id = TASK1001_ID;
    pSelf->base.desc_info = ZZ_STRING("This is only for develop\n");

    sts =  ZZApp_AddFlow(pApp, (zzTaskBaseST *)pSelf);
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

    //use default context
    sts  = ZZTaskBase_Init(pTaskBase, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_Init  error\n");
        goto END;
    }

    //create matrix 1001
    sts = ZZMatrix1001_Create(&pSelf->pMatrix1001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1001_Create  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(pTaskBase, &pSelf->pMatrix1001->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }


    //create matrix 2001
    sts = ZZMatrix2001_Create(&pSelf->pMatrix2001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2001_Create  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(pTaskBase, &pSelf->pMatrix2001->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }


    //create matrix 3001
    sts = ZZMatrix3001_Create(&pSelf->pMatrix3001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix3001_Create  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(pTaskBase, &pSelf->pMatrix3001->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }


    //create matrix 4001
    sts = ZZMatrix4001_Create(&pSelf->pMatrix4001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix4001_Create  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(pTaskBase, &pSelf->pMatrix4001->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
        goto END;
    }

    //create matrix 9001
    sts = ZZMatrix9001_Create(&pSelf->pMatrix9001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9001_Create  error\n");
        goto END;
    }

    //attach matrix to task
    sts = ZZTaskBase_AttachMatrix(pTaskBase, &pSelf->pMatrix9001->base);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTaskBase_AttachMatrix  error\n");
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

zzStatus ZZTask1001_Release(zzTaskBaseST *pTaskBase)
{
    zzStatus       sts   = ZZ_ERR_NONE;
    zzTask1001ST  *pSelf = GET_TASK1001(pTaskBase);

    //release martix 1001
    sts = ZZMatrix1001_Release(pSelf->pMatrix1001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1001_Release  error\n");
        goto END;
    }

    //release martix 2001
    sts = ZZMatrix2001_Release(pSelf->pMatrix2001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2001_Release  error\n");
        goto END;
    }

    //release martix 3001
    sts = ZZMatrix3001_Release(pSelf->pMatrix3001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix3001_Release  error\n");
        goto END;
    }

    //release martix 4001
    sts = ZZMatrix4001_Release(pSelf->pMatrix4001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix4001_Release  error\n");
        goto END;
    }

    //release martix 9001
    sts = ZZMatrix9001_Release(pSelf->pMatrix9001);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9001_Release  error\n");
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

