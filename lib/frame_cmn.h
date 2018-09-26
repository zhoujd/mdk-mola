/* zz_frame_reader.h ---
 */

#ifndef INCLUDED_ZZ_FRAME_OP_H
#define INCLUDED_ZZ_FRAME_OP_H 1

#include "cmn.h"

enum
{
    NOT_INIT_VALUE      =   0xFFF7
};

typedef struct _zzOwnFrameInfoST zzOwnFrameInfoST;
typedef struct _zzFrameData      zzFrameData;
typedef struct _zzFrameId        zzFrameId;
typedef struct _zzFrameInfo      zzFrameInfo;

enum inputParamEnum
{
    VPP_IN  = 0,
    VPP_IN_2,
    VPP_IN_3,
    VPP_IN_4,
    VPP_IN_5,
    VPP_IN_6,
    VPP_IN_7,
    VPP_IN_8,
    VPP_OUT,
    VPP_COUNT,
};


struct _zzOwnFrameInfoST
{
    zzU16  Width;
    zzU16  Pitch;
    zzU16  Height;
    // ROI
    zzU16  CropX;
    zzU16  CropY;
    zzU16  CropW;
    zzU16  CropH;
    zzU16  OutCropX;
    zzU16  OutCropY;
    zzU16  OutCropW;
    zzU16  OutCropH;
    zzU16  MultiSrcWorktaskflowId;
    zzU32  FourCC;
    zzU8   PicStruct;
    zzF64  dFrameRate;
};

/* Frame Data Info */
#define Frame_Y(p)    p->u1.Y
#define Frame_R(p)    p->u1.R

#define Frame_UV(p)   p->u2.UV
#define Frame_VU(p)   p->u2.VU
#define Frame_CbCr(p) p->u2.CbCr
#define Frame_CrCb(p) p->u2.CrCb
#define Frame_Cb(p)   p->u2.Cb
#define Frame_U(p)    p->u2.U
#define Frame_G(p)    p->u2.G

#define Frame_Cr(p)   p->u3.Cr
#define Frame_V(p)    p->u3.V
#define Frame_B(p)    p->u3.B

#define Frame_A(p)    p->u4.A

struct _zzFrameData
{
    zzU32      reserved[8];

    zzU64      TimeStamp;
    zzU32      FrameOrder;
    zzU16      Locked;
    zzU16      Pitch;

    /* color planes */
    union
    {
        zzU8   *Y;
        zzU8   *R;
    } u1;
    union
    {
        zzU8   *UV;            /* for UV merged formats */
        zzU8   *VU;            /* for VU merged formats */
        zzU8   *CbCr;          /* for CbCr merged formats */
        zzU8   *CrCb;          /* for CrCb merged formats */
        zzU8   *Cb;
        zzU8   *U;
        zzU8   *G;
    } u2;
    union
    {
        zzU8   *Cr;
        zzU8   *V;
        zzU8   *B;
    } u3;
    union
    {
        zzU8   *A;
    } u4;
    zzMemId    MemId;

    /* Additional Flags */
    zzU16  Corrupted;
    zzU16  DataFlag;
};

/* Frame ID for SVC and MVC */
struct _zzFrameId
{
    zzU16      TemporalId;
    zzU16      PriorityId;
    union
    {
        struct
        {
            zzU16  DependencyId;
            zzU16  QualityId;
        };
        struct
        {
            zzU32  ViewId;
        };
    };
};


/* Frame Info */
struct _zzFrameInfo
{
    zzU32  reserved[6];
    zzFrameId FrameId;

    zzU32  FourCC;
    zzU16  Width;
    zzU16  Pitch;
    zzU16  Height;

    zzU16  CropX;
    zzU16  CropY;
    zzU16  CropW;
    zzU16  CropH;

    zzU16  OutCropX;
    zzU16  OutCropY;
    zzU16  OutCropW;
    zzU16  OutCropH;

    zzU32  FrameRateExtN;
    zzU32  FrameRateExtD;
    zzU16  reserved3;

    zzU16  AspectRatioW;
    zzU16  AspectRatioH;

    zzU16  PicStruct;
    zzU16  ChromaFormat;
    zzU16  MultiSrcWorktaskflowId;
};


zzStatus ZZ_LoadNextFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
zzStatus ZZ_WriteFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst, zzBOOL bWithBox);

zzStatus ZZ_Str2FourCC(zz_char* strInput, zzU32 *pFourCC);
zz_char* ZZ_FourCC2Str(zzU32 fourcc);
zz_char* ZZ_PicStruct2Str(zzU8 picStruct);
zzStatus ZZ_DumpFrameInfo(zzFrameInfo *pFrameInfo);
zzStatus ZZ_ParseOwnFrameInfo(zzOwnFrameInfoST *frameInfo, int nArgNum, char **strInput);


#endif /* INCLUDED_ZZ_FRAME_OP_H */
