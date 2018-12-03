/* task_2002.h ---
 */

#ifndef INCLUDED_ZZ_TASK_2002_H
#define INCLUDED_ZZ_TASK_2002_H 1

#include "cmn.h"
#include "task_base.h"
#include "matrix_1002.h"
#include "matrix_2002.h"
#include "matrix_9001.h"
#include "lib/va_ctx_mng.h"
#include "lib/va_surf_mng.h"

#define TASK2002_ID  (2002)

typedef struct _zzTask2002ST zzTask2002ST;
typedef struct _zzTask2002InputParamsST zzTask2002InputParamsST;

enum Task2002Surface
{
    TASK2002_SCALING_SRC,
    TASK2002_SCALING_DST,

    TASK2002_SURF_COUNT,
};

struct _zzTask2002InputParamsST
{
    /* smart filters defined by mismatch btw src & dst */
    zzOwnFrameInfoST frameInfo[VPP_COUNT];// [0] - in, [1] - out

    zz_char  strSrcFile[MAX_FILENAME_LEN];
};

struct _zzTask2002ST
{
    zzTaskBaseST              base;
    zzVAContextST             ctx;

    zzSurfaceST               surface[TASK2002_SURF_COUNT];
    zzMatrix1002ST           *pMatrix1002;
    zzMatrix2002ST           *pMatrix2002;
    zzMatrix9001ST           *pMatrix9001;

    zzTask2002InputParamsST   params;

};

#define GET_TASK2002(p)  container_of(p, zzTask2002ST, base)

zzStatus ZZTask2002_Register();


#endif /* INCLUDED_ZZ_TASK_2002_H */
