/* matrix_2003.h ---
 */

#ifndef INCLUDED_ZZ_MATRIX_2003_H
#define INCLUDED_ZZ_MATRIX_2003_H 1

#include "cmn.h"
#include "matrix_defs.h"
#include "va_surf_mng.h"
#include "va_render_utils.h"
#include "va_render_parser.h"

#define MATRIX2003_ID    (2003)

typedef struct _zzMatrix2003ST zzMatrix2003ST;
typedef struct _zzMatrix2003InputParamsST zzMatrix2003InputParamsST;

typedef struct _zzMatrix2003InputParamsST  zzMatrix2003InputParamsST;
typedef struct _zzMatrix2003VpParamsST     zzMatrix2003VpParamsST;

struct _zzMatrix2003VpParamsST
{
    zzDeinterlaceParamST    deinterlace;
    zzSharpnessParamST      sharpness;
    zzAmpParamST            amp;
    zzDenoiseParamST        denoise;
    zzCompParamST           composition;

    //HDR
    zzHDRParamST            hdr_input;
    zzHDRParamST            hdr_output;
};


struct _zzMatrix2003InputParamsST
{
    zzMatrix2003VpParamsST  vp_params;
    zzU32                   rota_angle;
    zzU16                   hdr_type;
};


struct _zzMatrix2003ST
{
    zzMatrixBaseST                  base;

    zzVAContextST                  *ctx;

    zzSurfaceST                     src_surf;
    zzSurfaceST                     dst_surf;

    VABufferID                      filterBufs[VAProcFilterCount];
    zzU32                           numFilterBufs;

    VAProcPipelineParameterBuffer   pipelineParam;
    VABufferID                      pipelineParamID;

    VAProcPipelineParameterBuffer   subpicParam;
    VABufferID                      subpicParamID;

    zzBOOL                          bCompFlag;

    VAHdrMetaDataHDR10              out_metadata;
    VAHdrMetaData                   out_hdr_metadata;

    zzMatrix2003InputParamsST       params;
};


#define GET_MATRIX2003(p)  container_of(p, zzMatrix2003ST, base)

zzStatus ZZMatrix2003_Create(zzMatrix2003ST **ppRet);
zzStatus ZZMatrix2003_Release(zzMatrix2003ST *pSelf);

zzStatus ZZMatrix2003_Init(zzMatrix2003ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST *pPipeCtrl, zzVAContextST *pCtx,
                           zzSurfaceST *pSrcSurf, zzSurfaceST *pDstSurf);

zzStatus ZZMatrix2003_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix2003_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_2003_H */
