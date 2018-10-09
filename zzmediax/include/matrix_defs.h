/* matrix_defs.h
 *
 */

#ifndef INCLUDED_ZZ_PROC_DEFS_H
#define INCLUDED_ZZ_PROC_DEFS_H 1

#include "cmn.h"
#include "help.h"
#include "event.h"

typedef struct _zzMatrixBaseST  zzMatrixBaseST;
typedef struct _zzMatrixCellST  zzMatrixCellST;

typedef struct _zzPipeCtrlST zzPipeCtrlST;

typedef zzStatus   (*zzFunctionPreExec)(zzMatrixBaseST *pSelf);
typedef zzStatus   (*zzFunctionExec)(zzMatrixBaseST *pSelf);
typedef zzStatus   (*zzFunctionPostExec)(zzMatrixBaseST *pSelf);

struct _zzPipeCtrlST {
    zzEventID  pipe_event;
};

struct _zzMatrixBaseST
{
    zzU16               matrix_id;
    zzBOOL              restart_flag;

    zzMatrixCellST     *cell_list;
    zzU16               cell_num;

    zzEventID           next_event;    //to store event for the next
    zzPipeCtrlST       *pipe_ctrl;

    zz_list             matrix_list;
};

struct _zzMatrixCellST
{
    zzEventID  id;

    zzFunctionPreExec   pfnPreExec;
    zzFunctionExec      pfnExec;
    zzFunctionPostExec  pfnPostExec;
};

//matrix group functions


#endif /* INCLUDED_ZZ_PROC_DEFS_H */
