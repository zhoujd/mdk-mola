/* zz_frame_reader.h ---
 */

#ifndef INCLUDED_ZZ_FRAME_OP_H
#define INCLUDED_ZZ_FRAME_OP_H 1

#include "cmn.h"

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

/* Frame Data Info 10 bit */
#define Frame_Y16(p)    p->u1.Y16
#define Frame_R16(p)    p->u1.R16

#define Frame_UV16(p)   p->u2.UV16
#define Frame_VU16(p)   p->u2.VU16
#define Frame_CbCr16(p) p->u2.CbCr16
#define Frame_CrCb16(p) p->u2.CrCb16
#define Frame_Cb16(p)   p->u2.Cb16
#define Frame_U16(p)    p->u2.U16
#define Frame_G16(p)    p->u2.G16

#define Frame_Cr16(p)   p->u3.Cr16
#define Frame_V16(p)    p->u3.V16
#define Frame_B16(p)    p->u3.B16

#define Frame_A16(p)    p->u4.A16


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

        zzU16  *Y16;
        zzU16  *R16;
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

        zzU16  *UV16;            /* for UV merged formats */
        zzU16  *VU16;            /* for VU merged formats */
        zzU16  *CbCr16;          /* for CbCr merged formats */
        zzU16  *CrCb16;          /* for CrCb merged formats */
        zzU16  *Cb16;
        zzU16  *U16;
        zzU16  *G16;
    } u2;
    union
    {
        zzU8   *Cr;
        zzU8   *V;
        zzU8   *B;

        zzU16  *Cr16;
        zzU16  *V16;
        zzU16  *B16;

    } u3;
    union
    {
        zzU8   *A;
        zzU16  *A16;
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
};

zzStatus ZZ_LoadNextFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
zzStatus ZZ_WriteFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);

zzStatus ZZ_Str2FourCC(zz_char* strInput, zzU32 *pFourCC);
zz_char* ZZ_FourCC2Str(zzU32 fourcc);
zz_char* ZZ_PicStruct2Str(zzU8 picStruct);
zzStatus ZZ_DumpFrameInfo(zzFrameInfo *pFrameInfo);
zzStatus ZZ_ParseOwnFrameInfo(zzOwnFrameInfoST *frameInfo, int nArgNum, char **strInput);
zzStatus ZZ_ParamInfo2SurfInfo(zzFrameInfo *pSurfFrameInfo, zzOwnFrameInfoST *pParamFrameInfo);


#endif /* INCLUDED_ZZ_FRAME_OP_H */
