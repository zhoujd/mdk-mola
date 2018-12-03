/* task_2003.h ---
 */

#ifndef INCLUDED_ZZ_TASK_2003_H
#define INCLUDED_ZZ_TASK_2003_H 1

#include "cmn.h"
#include "task_base.h"
#include "matrix_1003.h"
#include "matrix_2002.h"
#include "matrix_9003.h"
#include "vaapi/va_ctx_mng.h"
#include "vaapi/va_surf_mng.h"

#define TASK2003_ID  (2003)

typedef struct _zzTask2003ST zzTask2003ST;
typedef struct _zzTask2003InputParamsST zzTask2003InputParamsST;

enum Task2003Surface
{
    TASK2003_SCALING_SRC,
    TASK2003_SCALING_DST,

    TASK2003_SURF_COUNT,
};

struct _zzTask2003InputParamsST
{
    /* smart filters defined by mismatch btw src & dst */
    zzOwnFrameInfoST frameInfo[VPP_COUNT];// [0] - in, [1] - out

    zz_char  strSrcFile[MAX_FILENAME_LEN];
};

struct _zzTask2003ST
{
    zzTaskBaseST              base;
    zzVAContextST             ctx;

    zzSurfaceST               surface[TASK2003_SURF_COUNT];
    zzMatrix1003ST           *pMatrix1003;
    zzMatrix2002ST           *pMatrix2002;
    zzMatrix9003ST           *pMatrix9003;

    zzTask2003InputParamsST   params;

};

#define GET_TASK2003(p)  container_of(p, zzTask2003ST, base)

zzStatus ZZTask2003_Register();


#endif /* INCLUDED_ZZ_TASK_2003_H */
