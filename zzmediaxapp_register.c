#include "zzmediaxapp_register.h"

zzStatus ZZMediaxApp_TaskRegister(zzAppST *pApp)
{
    zzStatus      sts    = ZZ_ERR_NONE;

    //init Task 2001
    sts = ZZTask2001_Register();
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1101_Regist error\n");
        goto END;
    }

#if 0
    //init Task 1001
    sts = ZZTask1001_Register();
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1001_Regist error\n");
        goto END;
    }
#endif

END:
    return sts;
}

zzStatus ZZMediaxApp_MatrixHelpRegister(zzAppST *pApp)
{
    zzStatus      sts    = ZZ_ERR_NONE;

    //init Matrix 1002
    sts = ZZMatrix1002_HelpRegister();
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1002_HelpRegister error\n");
        goto END;
    }


    //init Matrix 1002
    sts = ZZMatrix2002_HelpRegister();
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix2002_HelpRegister error\n");
        goto END;
    }

    //init Matrix 9002
    sts = ZZMatrix9002_HelpRegister();
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix9002_HelpRegister error\n");
        goto END;
    }


END:
    return sts;
}
