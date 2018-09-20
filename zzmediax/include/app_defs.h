/* app_defs.h --- 
 */

#ifndef ZZ_APP_DEF_H
#define ZZ_APP_DEF_H 1

#include "cmn.h"
#include "task_defs.h"
#include "matrix_defs.h"

#include "task_db.h"
#include "task_mng.h"
#include "dispatch.h"
#include "help.h"

typedef struct _zzAppST  zzAppST;
typedef zzStatus (*zzSelectTaskIdFP)(zzAppST *pSelf, zzU16 *pFlowID);
typedef zzStatus (*zzTaskRegisterFP)(zzAppST *pSelf);

//extern declare function
extern zzAppST * GetAPP();

#define GetWorkflowDB()         (&GetAPP()->task_db)
#define GetzTaskMng()           (&GetAPP()->task_mng)
#define GetDispatcher()         (&GetAPP()->dispatch)
#define GetHelp()               (&GetAPP()->help)

struct _zzAppST
{
    zzTaskDBST          task_db;
    zzTaskMngST         task_mng;
    zzDispatch          dispatch;
    zzHelpST            help;
    
    void                *data;   //for extend using
};

zzStatus ZZApp_Init(zzAppST                *pSelf,
                    zzU16                  argc,
                    zz_char                **argv,
                    zzSelectTaskIdFP    pfnSelectFlowID,
                    zzTaskRegisterFP    pfnTaskRegister,
                    zzHelpRegisterFP    pfnHelpRegister);
zzStatus ZZApp_Close(zzAppST *pSelf);
zzStatus ZZApp_Execute(zzAppST  *pSelf, zzU16 argc, zz_char **argv);


zzStatus ZZApp_AddFlow(zzAppST *pSelf, zzTaskBaseST *pFlowInfo);
zzStatus ZZApp_MatrixHelpAdd(zzAppST  *pSelf, zzHelpST *help_item);


zzStatus ZZApp_PrintHelp(zzAppST  *pSelf);
zzStatus ZZApp_TaskHelp(zzAppST *pSelf, zzU16 argc, zz_char **argv, zzU16 taskID);
zzStatus ZZApp_MatrixHelp(zzAppST *pSelf, zzU16 argc, zz_char **argv, zzU16 matrix_id);


#endif /* ZZ_APP_DEF_H */

