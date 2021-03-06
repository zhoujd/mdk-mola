/* task_db.h
 *
 */

#ifndef ZTASKDB_H
#define ZTASKDB_H

#include "cmn.h"
#include "task_base.h"

typedef struct _zzAppST     zzAppST;
typedef struct _zzTaskDBST  zzTaskDBST;
typedef zzStatus (*zzTaskRegisterFP)(zzAppST *pSelf);

struct _zzTaskDBST
{
    zz_list_head      task_head;
    zzTaskRegisterFP  pfnTaskRegister;
};

zzStatus ZZTaskDB_Init(zzTaskDBST *pSelf, zzU16 argc, zz_char** argv, zzTaskRegisterFP  pfnTaskRegister);
zzStatus ZZTaskDB_Close(zzTaskDBST *pSelf);

zzStatus ZZTaskDB_InitFlows(zzTaskDBST *pSelf, zzU16 argc, zz_char** argv);
zzStatus ZZTaskDB_ReleaseFlows(zzTaskDBST *pSelf);

zzStatus ZZTaskDB_AddFlow(zzTaskDBST *pSelf, zzTaskBaseST *pFlowInfo);
zzStatus ZZTaskDB_SelectFlow(zzTaskDBST *pSelf, zzU16 task_id, zzTaskBaseST **ppFlowInfo);


#endif //ZTASKDB_H
