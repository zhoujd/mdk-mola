/* task_1101.h --- 
 */

#ifndef INCLUDED_ZZ_TASK_1101_H
#define INCLUDED_ZZ_TASK_1101_H 1

#include "cmn.h"
#include "task_base.h"
#include "matrix_1101.h"
#include "va_ctx_mng.h"
#include "va_surf_mng.h"

#define TASK1101_ID  (1101)

enum Task1101Surface
{
    TASK1101_SCALING_SRC,
    TASK1101_SCALING_DST,

    TASK1101_SURF_COUNT,
};


typedef struct _zzTask1101ST zzTask1101ST;

struct _zzTask1101ST
{
    zzTaskBaseST            base;
    zzVAContextST           ctx;

    zzSurfaceST             surface[TASK1101_SURF_COUNT];
    zzMatrix1101ST          *pMatrix1101;
};

#define GET_TASK1101(p)  container_of(p, zzTask1101ST, base)

zzStatus ZZTask1101_Register();


#endif /* INCLUDED_ZZ_TASK_1101_H */

