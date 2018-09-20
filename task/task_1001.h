/* task_1001.h --- 
 */

#ifndef INCLUDED_ZZ_TASK_1001_H
#define INCLUDED_ZZ_TASK_1001_H 1

#include "cmn.h"
#include "task_base.h"
#include "matrix_1001.h"
#include "matrix_2001.h"
#include "matrix_3001.h"
#include "matrix_4001.h"
#include "matrix_9001.h"


#define TASK1001_ID  (1001)

typedef struct _zzTask1001ST zzTask1001ST;

struct _zzTask1001ST
{
    zzTaskBaseST    base;

    zzMatrix1001ST  *pMatrix1001;
    zzMatrix2001ST  *pMatrix2001;
    zzMatrix3001ST  *pMatrix3001;
    zzMatrix4001ST  *pMatrix4001;
    zzMatrix9001ST  *pMatrix9001;
};


#define GET_TASK1001(p)  container_of(p, zzTask1001ST, base)

zzStatus ZZTask1001_Register();


#endif /* INCLUDED_ZZ_TASK_1001_H */

