/* matrix_2002.h ---
 */

#ifndef INCLUDED_ZZ_MATRIX_2002_H
#define INCLUDED_ZZ_MATRIX_2002_H 1

#include "cmn.h"
#include "matrix_defs.h"
#include "lib/va_surf_mng.h"
#include "lib/va_render_utils.h"
#include "lib/va_render_parser.h"

#define MATRIX2002_ID    (2002)

typedef struct _zzMatrix2002ST zzMatrix2002ST;
typedef struct _zzMatrix2002InputParamsST zzMatrix2002InputParamsST;

typedef struct _zzMatrix2002InputParamsST  zzMatrix2002InputParamsST;
typedef struct _zzMatrix2002VpParamsST     zzMatrix2002VpParamsST;

struct _zzMatrix2002VpParamsST
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


struct _zzMatrix2002InputParamsST
{
    zzMatrix2002VpParamsST  vp_params;
    zzU32                   rota_angle;
    zzU16                   hdr_type;
};


struct _zzMatrix2002ST
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

    zzMatrix2002InputParamsST       params;
};


#define GET_MATRIX2002(p)  container_of(p, zzMatrix2002ST, base)

zzStatus ZZMatrix2002_Create(zzMatrix2002ST **ppRet);
zzStatus ZZMatrix2002_Release(zzMatrix2002ST *pSelf);

zzStatus ZZMatrix2002_Init(zzMatrix2002ST *pSelf, zzU16 argc, zz_char **argv,
                           zzPipeCtrlST *pPipeCtrl, zzVAContextST *pCtx,
                           zzSurfaceST *pSrcSurf, zzSurfaceST *pDstSurf);

zzStatus ZZMatrix2002_Help(zzHelpInfoST *pSelf, zzU16 argc, zz_char **argv);
zzStatus ZZMatrix2002_HelpRegister();


#endif /* INCLUDED_ZZ_MATRIX_2002_H */
