/* zzmediaxapp.h ---
 */

#ifndef ZZMEDIAXAPP_H
#define ZZMEDIAXAPP_H

#include "cmn.h"
#include "app_defs.h"

typedef struct _zzMediaxAppST      zzMediaxAppST;
typedef struct _zzMediaxAppParamST zzMediaxAppParamST;

struct _zzMediaxAppParamST
{
    zzU16 task_id;
};


struct  _zzMediaxAppST
{
    zzAppST            app;       //must be the first one
    zzMediaxAppParamST param;
};


zzStatus ZZMediaxApp_Init(zzMediaxAppST *pSelf, zzU16 argc, zz_char** argv);
zzStatus ZZMediaxApp_Close(zzMediaxAppST *pSelf);
zzStatus ZZMediaxApp_Help(zzMediaxAppST *pSelf, zzU16 argc, zz_char** argv);
zzStatus ZZMediaxApp_ParseInputString(zzMediaxAppST *pSelf, zzU16 argc, zz_char** argv);

zzStatus ZZMediaxApp_SelectFlowID(zzAppST *pZZApp, zzU16 *pFlowID);

zzStatus ZZMediaxApp_TaskRegister(zzAppST *pSelf);
zzStatus ZZMediaxApp_MatrixHelpRegister(zzAppST  *pSelf);



#endif //ZZMEDIAXAPP
