#include "zzmediaxapp_register.h"

zzStatus ZZMediaxApp_TaskRegister(zzAppST *pApp)
{
    zzStatus      sts    = ZZ_ERR_NONE;

    //init Task 1101
    sts = ZZTask1101_Register();
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1101_Regist error\n");
        goto END;
    }

    //init Task 1001
    sts = ZZTask1001_Register();
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZTask1001_Regist error\n");
        goto END;
    }


END:
    return sts;
}

zzStatus ZZMediaxApp_MatrixHelpRegister(zzAppST *pApp)
{
    zzStatus      sts    = ZZ_ERR_NONE;

    //init Matrix 1101
    sts = ZZMatrix1101_HelpRegister();
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZMatrix1101_HelpRegister error\n");
        goto END;
    }


END:
    return sts;
}
