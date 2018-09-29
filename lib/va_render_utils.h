/* va_render_utils.h ---
 */

#ifndef INCLUDED_ZZ_RENDER_UTILS_H
#define INCLUDED_ZZ_RENDER_UTILS_H 1

#include "cmn.h"
#include "utils.h"
#include <va/va.h>
#include <va/va_vpp.h>


#define ZZ_VP_DEF_COLOR_FILL 0xff108080

typedef enum _enParamSetFlag enParamSetFlag;
typedef enum _enParamType    enParamType;

typedef struct _zzRendBaseST           zzRendBaseST;
typedef struct _zzSharpnessParamST     zzSharpnessParamST;
typedef struct _zzAmpParamST           zzAmpParamST;
typedef struct _zzDenoiseParamST       zzDenoiseParamST;
typedef struct _zzDeinterlaceParamST   zzDeinterlaceParamST;
typedef struct _zzRotationParamST      zzRotationParamST;
typedef struct _zzCompParamST          zzCompParamST;

typedef struct _zzFRCParamST           zzFRCParamST;
typedef struct _zzScalingParamST       zzScalingParamST;
typedef struct _zzDIADVParamST         zzDIADVParamST;
typedef struct _zzToneMappingParamST   zzToneMappingParamST;

enum _enParamSetFlag
{
    VP_FILTER_UNSET = 0,
    VP_FILTER_SET   = 1,
};

enum _enParamType
{
    RENDER_SCALING_EN = 0,
    RENDER_SHARPNESS_EN,
    RENDER_AMP_EN,
    RENDER_DENOISE_EN,
    RENDER_DEINTERLACE_EN,
    RENDER_ROTATION_EN,
    RENDER_COMP_EN,

    RENDER_MAX_COUNT,
};

struct _zzRendBaseST
{
    enParamType    type;
    zzU8           flag;
};

struct _zzSharpnessParamST
{
    zzRendBaseST   base;
    zzU16          ief;
    zzU16          factor;
};


struct _zzAmpParamST
{
    zzRendBaseST   base;
    zzF64          brightness;
    zzF64          contrast;
    zzF64          saturation;
    zzF64          hue;

};

struct _zzDenoiseParamST
{
    zzRendBaseST   base;
    zzU16          factor;
};

struct _zzDeinterlaceParamST
{
    zzRendBaseST                 base;
    VAProcDeinterlacingType      type;

    zzU32                        bff_flag;      //bottom field first
    zzU32                        btm_field_flag;
    zzU32                        one_field_flag;
};

struct _zzRotationParamST
{
    zzRendBaseST   base;
    zzU16          angle;
};

struct _zzCompParamST
{
    zzRendBaseST   base;
    zzU16          row_num;
    zzU16          col_num;
    zzF32          comp_alpha;
};


#define ZZ_FRC_MAX_REF_NUM        3
#define ZZ_FRC_24To60_REF_NUM     3
#define ZZ_FRC_30To60_REF_NUM     2
#define ZZ_FRC_24To60_OUT_FRAMES  5
#define ZZ_FRC_30To60_OUT_FRAMES  2

enum FRCCycleSize
{
    FRC_24To60_CYCLE_SIZE = 4,
    FRC_30To60_CYCLE_SIZE = 3,
};

enum
{
    FRC_INPUT_FPS24 = 24,
    FRC_INPUT_FPS30 = 30,
};

enum
{
    FRC_OUTPUT_FPS60 = 60,
    FRC_OUTPUT_Count,
};


struct _zzFRCParamST
{
    zzRendBaseST     base;
    VASurfaceID      src_surface;
    VASurfaceID      target_surface;
    zzU32            input_fps;
    zzU32            output_fps;

    zzU32            num_refs;
    VASurfaceID      pRefID[ZZ_FRC_MAX_REF_NUM];

    zzU32            num_frameIDs;
    zzU32            frameID[ZZ_FRC_MAX_REF_NUM];

    VARectangle      primary_src;
    VARectangle      primary_dst;
};

struct _zzScalingParamST
{
    zzRendBaseST     base;
    VASurfaceID      src_surface;
    VASurfaceID      target_surface;
    VARectangle      primary_src;
    VARectangle      primary_dst;
};


struct _zzDIADVParamST
{
    zzRendBaseST     base;
    VASurfaceID      src_surface;
    VASurfaceID      target_surface;
    zzU32            num_refs;
    VASurfaceID      pRefID[ZZ_FRC_MAX_REF_NUM];
    VARectangle      primary_src;
    VARectangle      primary_dst;
};

struct _zzToneMappingParamST
{
    //zhoujd
};

#define ZZ_VP_MAX_FILTER     9
#define ZZ_MAX_REF_NUM       32

typedef struct _zzPipelineBufferST     zzPipelineBufferST;

struct _zzPipelineBufferST
{
    VASurfaceID             surface;
    VARectangle            *src;
    VARectangle            *dst;
    VABufferID              filters[ZZ_VP_MAX_FILTER];
    zzU32                   filter_num;
    zzU32                   filter_flags;
    zzU32                   color_fill;
    VAProcColorStandardType surface_color_standard;
    VAProcColorStandardType output_color_standard;

    VASurfaceID             forward_references[ZZ_MAX_REF_NUM];
    unsigned int            num_forward_references;
    VASurfaceID             backward_references[ZZ_MAX_REF_NUM];
    unsigned int            num_backward_references;
};

zzStatus render_picture_vp_check_pipeline(VADisplay display, VAContextID ctx_id);
zzStatus render_picture_vp_primary(VADisplay display, VAContextID ctx_id, zzPipelineBufferST *pParams, VABufferID *primary_buf_id);
zzStatus render_picture_vp_subpicture(VADisplay  display, VAContextID ctx_id, zzPipelineBufferST *pParams, VABufferID *subpicture_buf_id);

zzStatus render_picture_vp_di(VADisplay display, VAContextID ctx_id, VABufferID *deint_buf_id, zzDeinterlaceParamST *pParam);
zzStatus render_picture_vp_frc(VADisplay display, VAContextID ctx_id, VABufferID *frc_buf_id, zzU32 input_fps, zzU32 output_fps, zzU32 num_refs, VASurfaceID *pID);
zzStatus render_picture_vp_ief(VADisplay display, VAContextID ctx_id, VABufferID *sharp_buf_id, zzU16 ief_factor);
zzStatus render_picture_vp_hsbc(VADisplay display, VAContextID ctx_id, VABufferID *hsbc_buf_id, zzF64 hue, zzF64 saturation, zzF64 brightness, zzF64 contrast);
zzStatus render_picture_vp_dn(VADisplay display, VAContextID ctx_id, VABufferID *noise_buf_id, zzU64 denoise);
zzStatus render_picture_vp_tone(VADisplay display, VAContextID ctx_id, VABufferID *tone_mapping_id);



#endif /* INCLUDED_ZZ_RENDER_UTILS_H */
