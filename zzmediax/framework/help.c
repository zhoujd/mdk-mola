/* help.c --- 
 */


#include "help.h"
#include "app_base.h"

zzStatus ZZHelp_Init(zzHelpST *pSelf, zzU16 argc, zz_char **argv, zzHelpRegisterFP fpHelpRegister)
{
    zzStatus sts = ZZ_ERR_NONE;

    pSelf->fpHelpRegister = fpHelpRegister;

    //register task
    if (NULL != pSelf->fpHelpRegister)
    {
        sts = pSelf->fpHelpRegister(GetAPP());
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZTaskDB TaskRegister error\n");
            goto END;
        }
    }


    pSelf->bHelpFlag = FALSE;

END:
    return sts;
}

zzStatus ZZHelp_Close(zzHelpST *pSelf)
{
    zzStatus  sts    = ZZ_ERR_NONE;
    zz_list   *pos   = NULL;
    zz_list   *n     = NULL; 

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);
    
    //free task list
    list_for_each_safe(pos, n, &pSelf->helpinfo_head)
    {
        zzHelpInfoST *pTemp = container_of(pos, zzHelpInfoST, helpinfo_list);
        list_del_init(pos);
        FREEIF(pTemp);
    }

    return sts;
}

zzStatus ZZHelp_MatrixHelpAdd(zzHelpST *pSelf, zzHelpInfoST *pHelpItem)
{
    zzStatus  sts     = ZZ_ERR_NONE;

    INIT_LIST_HEAD(&pSelf->helpinfo_head);
    list_add_tail(&pHelpItem->helpinfo_list, &pSelf->helpinfo_head);
    
    return sts;
}


zzStatus ZZHelp_MatrixHelp(zzHelpST               *pSelf,
                           zzU16                  argc,
                           zz_char                **argv,
                           zzU16                  matrix_id)
{
    zzStatus  sts       = ZZ_ERR_NONE;
    zzBOOL    bFind     = FALSE;
    zz_list*  pos       = NULL; 

    ZZPRINTF("proc help root for %d\n", matrix_id);

    list_for_each(pos, &pSelf->helpinfo_head)
    {
        zzHelpInfoST *pTemp = container_of(pos, zzHelpInfoST, helpinfo_list);
        if (pTemp->help_id == matrix_id)
        {
            bFind = TRUE;
            
            if (NULL != pTemp->pfnMatrixHelpFn)
            {
                sts = pTemp->pfnMatrixHelpFn(pTemp, argc, argv);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("ZZHelp MatrixHelpFn for %d error\n", matrix_id);
                    goto END;
                }                
            }
            else
            {
                ZZPRINTF("ZZHelp pfnMatrixHelpFn == NULL %d error\n", matrix_id);
            }
            
            break;
        }
    }

    if (FALSE == bFind)
    {
        ZZPRINTF("help for %d does not exist or Have you register it in your Register Funciton? :)\n", matrix_id);
    }
    
END:
    return sts;
}
