/* task_mng.h
 *
 */

#ifndef TASKMNG_H
#define TASKMNG_H

#include "cmn.h"
#include "task_base.h"

typedef struct _zzTaskMngST zzTaskMngST;

struct _zzTaskMngST
{
    zzTaskBaseST     *curr_task;

    zz_list_head      exec_task_head;
};

zzStatus ZZTaskMng_Init(zzTaskMngST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZTaskMng_Execute(zzTaskMngST *pSelf, zzU16 argc, zz_char **argv, zzTaskBaseST *pFlowInfo);
zzStatus ZZTaskMng_Close(zzTaskMngST *pSelf);

//For Async execute task
zzStatus ZZTaskMng_StartTask(zzTaskMngST *pSelf, zzU16 argc, zz_char **argv, zzTaskBaseST *pFlowInfo);
zzStatus ZZTaskMng_EndTask(zzTaskMngST *pSelf);
zzStatus ZZTaskMng_ExecStartTask(zzTaskMngST *pSelf);
zzStatus ZZTaskMng_ExecEndTask(zzTaskMngST *pSelf);
zzBOOL   ZZTaskMng_CheckTaskInExecList(zzTaskMngST *pSelf, zzU16 taskID);

#endif //TASKMNG_H
