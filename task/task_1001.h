/* task_1001.h ---
 */

#ifndef INCLUDED_ZZ_TASK_1001_H
#define INCLUDED_ZZ_TASK_1001_H 1

#include "cmn.h"
#include "task_base.h"
#include "matrix_1002.h"
#include "matrix_9002.h"
#include "va_ctx_mng.h"
#include "va_surf_mng.h"

#define TASK1001_ID  (1001)

typedef struct _zzTask1001ST zzTask1001ST;
typedef struct _zzTask1001InputParamsST zzTask1001InputParamsST;

enum Task1001Surface
{
    TASK1001_SCALING_SRC,
    TASK1001_SURF_COUNT,
};

struct _zzTask1001InputParamsST
{
    zzOwnFrameInfoST frameInfo[VPP_COUNT];// [0] - in, [1] - out

    zz_char          strSrcFile[MAX_FILENAME_LEN];
    zz_char          strDstFile[MAX_FILENAME_LEN];

    zzU16            numFrames;

};

struct _zzTask1001ST
{
    zzTaskBaseST            base;
    zzVAContextST           ctx;

    zzSurfaceST             surface[TASK1001_SURF_COUNT];
    zzMatrix1002ST          *pMatrix1002;
    zzMatrix9002ST          *pMatrix9002;

    zzBOOL                    screen_flag;
    zzTask1001InputParamsST   params;

};

#define GET_TASK1001(p)  container_of(p, zzTask1001ST, base)

zzStatus ZZTask1001_Register();


#endif /* INCLUDED_ZZ_TASK_1001_H */
