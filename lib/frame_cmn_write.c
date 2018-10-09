/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)raw_write.c
 */

#include "frame_cmn.h"

static zzStatus ZZ_Write_YV12(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_IYUV(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_400P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_411P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);

static zzStatus ZZ_Write_444P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_IMC3(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_422H(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_422V(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);

static zzStatus ZZ_Write_NV12(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_P010(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_YUY2(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);

static zzStatus ZZ_Write_ARGB_XRGB(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_ABGR(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_R5G6B5(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
static zzStatus ZZ_Write_A2RGB10(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);

static struct zzFrameWriteMap
{
    zzU32    Fourcc;
    zzStatus (*fnWrite_func)(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
} frame_write_map[] = {
    {ZZ_FOURCC_YV12,    ZZ_Write_YV12},
    {ZZ_FOURCC_IYUV,    ZZ_Write_IYUV},
    {ZZ_FOURCC_400P,    ZZ_Write_400P},
    {ZZ_FOURCC_411P,    ZZ_Write_411P},

    {ZZ_FOURCC_444P,    ZZ_Write_444P},
    {ZZ_FOURCC_IMC3,    ZZ_Write_IMC3},
    {ZZ_FOURCC_422H,    ZZ_Write_422H},
    {ZZ_FOURCC_422V,    ZZ_Write_422V},

    {ZZ_FOURCC_NV12,    ZZ_Write_NV12},
    {ZZ_FOURCC_P010,    ZZ_Write_P010},
    {ZZ_FOURCC_YUY2,    ZZ_Write_YUY2},

    {ZZ_FOURCC_ARGB,    ZZ_Write_ARGB_XRGB},
    {ZZ_FOURCC_XRGB,    ZZ_Write_ARGB_XRGB},
    {ZZ_FOURCC_ABGR,    ZZ_Write_ABGR},
    {ZZ_FOURCC_R5G6B5,  ZZ_Write_R5G6B5},
    {ZZ_FOURCC_A2RGB10, ZZ_Write_A2RGB10},
};

zzStatus ZZ_WriteFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzU8   i  = 0;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    for (i = 0; i < ARRAY_NUM(frame_write_map); i++)
    {
        if (pInfo->FourCC == frame_write_map[i].Fourcc)
        {
            return frame_write_map[i].fnWrite_func(pData, pInfo, fDst);
        }
    }

    return ZZ_ERR_UNSUPPORTED;
}

zzStatus ZZ_Write_YV12(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  nBytesRead   = 0;

    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;
    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL(zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    w     >>= 1;
    h     >>= 1;
    pitch >>= 1;

    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY >> 1) * pitch;

    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY >> 1) * pitch;

    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fwrite(ptr + i * pitch, 1, w, fDst);
        CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }

    return ZZ_ERR_NONE;

}

zzStatus ZZ_Write_IYUV(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  nBytesRead   = 0;

    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;

    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL(zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    w     >>= 1;
    h     >>= 1;
    pitch >>= 1;

    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY >> 1) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fwrite(ptr + i * pitch, 1, w, fDst);
        CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }

    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY >> 1) * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    return ZZ_ERR_NONE;
}


zzStatus ZZ_Write_400P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;

    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL(zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }


    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_411P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  nBytesRead   = 0;

    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;

    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL(zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    w >>= 2;
    pitch >>= 2;
    ptr  = Frame_U(pData) + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fwrite(ptr + i * pitch, 1, w, fDst);
        CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }

    ptr  = Frame_V(pData) + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_444P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  nBytesRead   = 0;

    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;

    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL(zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    ptr  = Frame_U(pData) + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fwrite(ptr + i * pitch, 1, w, fDst);
        CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }

    ptr  = Frame_V(pData) + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_IMC3(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  nBytesRead   = 0;

    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;


    static int count = 1;
    printf("%s: %s frame=[%d] w=[%d] h=[%d] pitch=[%d]\n", __FUNCTION__, "ZZ_FOURCC_IMC3", count, w, h, pitch);
    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;

    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL(zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    h >>= 1;

    ptr  = Frame_U(pData) + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fwrite(ptr + i * pitch, 1, w, fDst);
        CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }

    ptr  = Frame_V(pData) + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        //            CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ (i + 16) * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }
    count ++;

    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_422H(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  nBytesRead   = 0;

    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;

    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL(zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    w     >>= 1;
    pitch >>= 1;

    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fwrite(ptr + i * pitch, 1, w, fDst);
        CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }

    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_422V(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  nBytesRead   = 0;

    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;
    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL(zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    h >>= 1;

    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fwrite(ptr + i * pitch, 1, w, fDst);
        CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }


    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_NV12(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;
    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    // write UV data
    h     >>= 1;
    ptr  = Frame_UV(pData) + (pInfo->CropX ) + (pInfo->CropY >> 1) * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }


    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_P010(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;
    for (i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr + i * pitch, 1, w * 2, fDst), w * 2, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    // write UV data
    h     >>= 1;
    ptr  = Frame_UV(pData) + (pInfo->CropX ) + (pInfo->CropY >> 1) * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr + i * pitch, 1, w * 2, fDst), w * 2, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }


    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_YUY2(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, 2*w, fDst), 2*w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }


    return ZZ_ERR_NONE;
}


zzStatus ZZ_Write_ARGB_XRGB(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    CHECK_POINTER(Frame_R(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_G(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_B(pData), ZZ_ERR_NOT_INITIALIZED);
    // there is issue with A channel in case of d3d, so A-ch is ignored
    //CHECK_POINTER(pData->A, ZZ_ERR_NOT_INITIALIZED);

    ptr = ZZ_MIN( ZZ_MIN(Frame_R(pData), Frame_G(pData)), Frame_B(pData) );
    ptr = ptr + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr + i * pitch, 1, 4*w, fDst), 4*w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }


    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_ABGR(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    CHECK_POINTER(Frame_R(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_G(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_B(pData), ZZ_ERR_NOT_INITIALIZED);
    // there is issue with A channel in case of d3d, so A-ch is ignored
    //CHECK_POINTER(pData->A, ZZ_ERR_NOT_INITIALIZED);

    ptr = ZZ_MIN( ZZ_MIN(Frame_R(pData), Frame_G(pData)), Frame_B(pData) );
    ptr = ptr + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr + i * pitch, 1, 4*w, fDst), 4*w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }


    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_R5G6B5(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    CHECK_POINTER(Frame_R(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_G(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_B(pData), ZZ_ERR_NOT_INITIALIZED);
    // there is issue with A channel in case of d3d, so A-ch is ignored
    //CHECK_POINTER(pData->A, ZZ_ERR_NOT_INITIALIZED);

    ptr = ZZ_MIN( ZZ_MIN(Frame_R(pData), Frame_G(pData)), Frame_B(pData));
    ptr = ptr + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr + i * pitch, 1, 2*w, fDst), 2*w, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }

    return ZZ_ERR_NONE;
}

zzStatus ZZ_Write_A2RGB10(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    CHECK_POINTER(Frame_R(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_G(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_B(pData), ZZ_ERR_NOT_INITIALIZED);
    // there is issue with A channel in case of d3d, so A-ch is ignored
    //CHECK_POINTER(pData->A, ZZ_ERR_NOT_INITIALIZED);

    ptr = ZZ_MIN( ZZ_MIN(Frame_R(pData), Frame_G(pData)), Frame_B(pData) );
    ptr = ptr + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        CHECK_NOT_EQUAL( zz_file_fwrite(ptr + i * pitch, 1, 4*w*2, fDst), 4*w*2, ZZ_ERR_UNDEFINED_BEHAVIOR);
    }


    return ZZ_ERR_NONE;
}
