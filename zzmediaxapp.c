/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)zzmediaxapp.c
 */

#include "zzmediaxapp.h"
#include "help.h"


zzStatus ZZMediaxApp_Init(zzMediaxAppST *pSelf, zzU16 argc, zz_char** argv)
{
    zzStatus  sts = ZZ_ERR_NONE;

    //init zz base application
    sts = ZZApp_Init(GetAPP(), argc, argv,
                     ZZMediaxApp_SelectFlowID,
                     ZZMediaxApp_TaskRegister,
                     ZZMediaxApp_MatrixHelpRegister);

    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_Init error\n");
        goto END;
    }

    //parse Input param
    sts = ZZMediaxApp_ParseInputString(pSelf, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMediaxApp_ParseInputString error\n");
        goto END;
    }

END:
    return sts;
}

zzStatus ZZMediaxApp_Close(zzMediaxAppST *pSelf)
{
    zzStatus  sts = ZZ_ERR_NONE;

    sts = ZZApp_Close(GetAPP());
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZApp_Close error\n");
        goto END;
    }

END:
    return sts;

}

zzStatus ZZMediaxApp_Help(zzMediaxAppST *pSelf, zzU16 argc, zz_char** argv)
{
    zzStatus  sts = ZZ_ERR_NONE;

    if (argc == 2 || argc == 1)
    {
        //parse Input param
        sts = ZZApp_PrintHelp(GetAPP());
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZApp_PrintHelp error\n");
            goto END;
        }
    }
    else
    {
        if (0 == zz_strcmp(argv[2], ZZ_STRING("-task")))
        {
            if (argc > 3)
            {
                pSelf->param.task_id = zz_atoi(argv[3]);

                //init zz base application
                sts = ZZApp_TaskHelp(GetAPP(), argc, argv, pSelf->param.task_id);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("ZZApp_HelpTask error\n");
                    goto END;
                }
            }
            else
            {
                ZZPRINTF("Please input task id after -task\n");
            }
        }
        else if (0 == zz_strcmp(argv[2], ZZ_STRING("-matrix")))
        {
            if (argc > 3)
            {
                zzU16 matrix_id = 0;
                matrix_id = zz_atoi(argv[3]);

                //help proc
                sts = ZZApp_MatrixHelp(GetAPP(), argc, argv, matrix_id);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("ZZApp_HelpTask error\n");
                    goto END;
                }
            }
            else
            {
                ZZPRINTF("Please input proc id after -proc\n");
            }

        }
        else
        {
            ZZPRINTF("ZZuarder help unsupported param\n");
            goto END;
        }

    }

END:
    return sts;

}

zzStatus ZZMediaxApp_SelectFlowID(zzAppST *pZZApp, zzU16 *pFlowID)
{
    zzStatus     sts     = ZZ_ERR_NONE;
    zzMediaxAppST *pSelf = container_of(pZZApp, zzMediaxAppST, app);

    *pFlowID = pSelf->param.task_id;

    return sts;
}

zzStatus ZZMediaxApp_ParseInputString(zzMediaxAppST *pSelf, zzU16 argc, zz_char** argv)
{
    zzStatus  sts  = ZZ_ERR_NONE;
    int       i    = 0;
    zzBOOL    flag = FALSE;

    //default for help
    if (1 == argc)
    {
        pSelf->app.help.bHelpFlag = TRUE;
        flag = TRUE;
    }

    for (i = 1; i < argc; i++)
    {
        if (0 == zz_strcmp(argv[i], ZZ_STRING("-task")))
        {
            i++;
            if (argv[i] != NULL)
            {
                pSelf->param.task_id = zz_atoi(argv[i]);
                flag = TRUE;
                break;
            }
        }
        else if (0 == zz_strcmp(argv[i], ZZ_STRING("help")))
        {
            pSelf->app.help.bHelpFlag = TRUE;
            flag = TRUE;
            break;
        }
    }

    if (flag != TRUE)
    {
        ZZPRINTF("please use -task task_id parameter\n");
        sts = ZZ_ERR_NOT_FOUND;
        goto END;
    }

END:
    return sts;

}
