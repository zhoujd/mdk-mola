/* help.h ---
 */

#ifndef INCLUDED_ZZ_HELP_H
#define INCLUDED_ZZ_HELP_H 1

#include "cmn.h"

typedef struct _zzAppST      zzAppST;
typedef struct _zzHelpST     zzHelpST;
typedef struct _zzHelpInfoST zzHelpInfoST;

typedef zzStatus (*zzMatrixHelpFn)(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
typedef zzStatus (*zzHelpRegisterFP)(zzAppST *pSelf);

struct _zzHelpInfoST
{
    zzU16                 help_id;
    zz_char               *desc_info;
    zzMatrixHelpFn        pfnMatrixHelpFn;

    zz_list               helpinfo_list;
};

struct _zzHelpST
{
    zzBOOL                bHelpFlag;
    zz_list_head          helpinfo_head;

    zzHelpRegisterFP      fpHelpRegister;
};

zzStatus ZZHelp_Init(zzHelpST *pSelf, zzU16 argc, zz_char **argv, zzHelpRegisterFP fpHelpRegister);
zzStatus ZZHelp_Close(zzHelpST *pSelf);
zzStatus ZZHelp_MatrixHelpAdd(zzHelpST *pSelf, zzHelpInfoST *help_item);

zzStatus ZZHelp_MatrixHelp(zzHelpST               *pSelf,
                           zzU16                  argc,
                           zz_char                **argv,
                           zzU16                  matrix_id);

#endif /* INCLUDED_ZZ_HELP_H */
