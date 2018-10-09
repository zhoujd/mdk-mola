/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)main.c
 */

#include "config.h"
#include "app_defs.h"
#include "zzmediaxapp.h"

//global definations
static ZZMEDIAXAPP_ST *g_zzApp;
zzAppST * GetAPP() {return (zzAppST *)g_zzApp;}

#if defined(_WIN32) || defined(_WIN64)
int _tmain(int argc, TCHAR *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    zzStatus sts = ZZ_ERR_NONE;

    ZZPRINTF("Welcome to zzmediax, Have fun && Good luck!\n");

    g_zzApp = (ZZMEDIAXAPP_ST *)AllocAndZeroMem(sizeof(ZZMEDIAXAPP_ST));
    if (g_zzApp == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }

    sts = ZZMEDIAX_INIT(g_zzApp, argc, argv);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ_MEDIAX_INIT  error\n");
        goto END;
    }

    if (GetAPP()->help.bHelpFlag == TRUE)
    {
        sts = ZZMEDIAX_HELP(g_zzApp, argc, argv);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZ_MEDIAX_HELP  error\n");
            goto END;
        }
    }
    else
    {
        sts = ZZApp_Execute(GetAPP(), argc, argv);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ZZApp_Execute  error\n");
            goto END;
        }
    }

    sts = ZZMEDIAX_CLOSE(g_zzApp);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("ZZ fnRelease  error\n");
        goto END;
    }

END:
    FREEIF(g_zzApp);

    return sts;

}
