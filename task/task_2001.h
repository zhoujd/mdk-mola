/* task_2001.h ---
 */

#ifndef INCLUDED_ZZ_TASK_2001_H
#define INCLUDED_ZZ_TASK_2001_H 1

#include "cmn.h"
#include "task_base.h"
#include "matrix_1002.h"
#include "matrix_2002.h"
#include "matrix_9002.h"
#include "va_ctx_mng.h"
#include "va_surf_mng.h"

#define TASK2001_ID  (2001)

typedef struct _zzTask2001ST zzTask2001ST;
typedef struct _zzTask2001InputParamsST zzTask2001InputParamsST;

enum Task2001Surface
{
    TASK2001_SCALING_SRC,
    TASK2001_SCALING_DST,

    TASK2001_SURF_COUNT,
};

struct _zzTask2001InputParamsST
{
    /* smart filters defined by mismatch btw src & dst */
    zzOwnFrameInfoST frameInfo[VPP_COUNT];// [0] - in, [1] - out

    zz_char  strSrcFile[MAX_FILENAME_LEN];
    zz_char  strDstFile[MAX_FILENAME_LEN];

};

struct _zzTask2001ST
{
    zzTaskBaseST              base;
    zzVAContextST             ctx;

    zzSurfaceST               surface[TASK2001_SURF_COUNT];
    zzMatrix1002ST           *pMatrix1002;
    zzMatrix2002ST           *pMatrix2002;
    zzMatrix9002ST           *pMatrix9002;

    zzTask2001InputParamsST   params;

};

#define GET_TASK2001(p)  container_of(p, zzTask2001ST, base)

zzStatus ZZTask2001_Register();


#endif /* INCLUDED_ZZ_TASK_2001_H */
