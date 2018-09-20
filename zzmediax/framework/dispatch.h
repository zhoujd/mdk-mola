#ifndef ZDISPATCHERBASE_H
#define ZDISPATCHERBASE_H

#include "cmn.h"

typedef struct _zzAppST    zzAppST;
typedef struct _zzDispatch zzDispatch;


typedef zzStatus (*zzSelectFlowID)(zzAppST *pZZApp, zzU16 *pFlowID);


struct _zzDispatch
{
    zzSelectFlowID  pfnSelectFlowID;
};

zzStatus ZZDispatch_Init(zzDispatch *pSelf, zzU16 argc, zz_char **argv, zzSelectFlowID  pfnSelectFlowID);
zzStatus ZZDispatch_Run(zzDispatch *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZDispatch_Close(zzDispatch *pSelf);



#endif //ZDISPATCHERBASE_H