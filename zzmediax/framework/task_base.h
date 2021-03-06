/* task_base.h
 *
 */

#ifndef ZWORKFLOWBASE_H
#define ZWORKFLOWBASE_H

#include "cmn.h"
#include "task_defs.h"

zzStatus ZZTaskBase_Init(zzTaskBaseST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZTaskBase_ExecInit(zzTaskBaseST *pSelf);

zzStatus ZZTaskBase_PreExec(zzTaskBaseST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZTaskBase_Exec(zzTaskBaseST *pSelf);
zzStatus ZZTaskBase_PostExec(zzTaskBaseST *pSelf);

zzStatus ZZTaskBase_Release(zzTaskBaseST *pSelf);
zzStatus ZZTaskBase_Help(zzTaskBaseST *pSelf, zzU16 argc, zz_char **argv);


#endif //ZWORKFLOWBASE_H
