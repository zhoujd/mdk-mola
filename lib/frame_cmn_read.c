/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)raw_read.c
 */

#include "frame_cmn.h"

static zzStatus ZZ_LoadNext_YV12(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_IYUV(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_422H(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_422V(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_IMC3(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_400P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_411P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_444P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_NV12(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_P010(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_NV12_LINEAR(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_ABGR(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_ARGB_XRGB(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_R5G6B5(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);
static zzStatus ZZ_LoadNext_YUY2(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc);


static struct zzFrameReadMap
{
    zzU32    Fourcc;
    zzStatus (*fnRead_func)(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst);
} frame_read_map[] = {
    {ZZ_FOURCC_YV12,        ZZ_LoadNext_YV12},
    {ZZ_FOURCC_IYUV,        ZZ_LoadNext_IYUV},
    {ZZ_FOURCC_422H,        ZZ_LoadNext_422H},
    {ZZ_FOURCC_422V,        ZZ_LoadNext_422V},
    {ZZ_FOURCC_IMC3,        ZZ_LoadNext_IMC3},
    
    {ZZ_FOURCC_400P,        ZZ_LoadNext_400P},
    {ZZ_FOURCC_411P,        ZZ_LoadNext_411P},
    {ZZ_FOURCC_444P,        ZZ_LoadNext_444P},
                            
    {ZZ_FOURCC_NV12,        ZZ_LoadNext_NV12},
    {ZZ_FOURCC_P010,        ZZ_LoadNext_P010},
    {ZZ_FOURCC_NV12_LINEAR, ZZ_LoadNext_NV12_LINEAR},

    {ZZ_FOURCC_ABGR,        ZZ_LoadNext_ABGR},
    {ZZ_FOURCC_ARGB,        ZZ_LoadNext_ARGB_XRGB},
    {ZZ_FOURCC_XRGB,        ZZ_LoadNext_ARGB_XRGB},
    {ZZ_FOURCC_R5G6B5,      ZZ_LoadNext_R5G6B5},

    {ZZ_FOURCC_YUY2,        ZZ_LoadNext_YUY2},
};

zzStatus ZZ_LoadNextFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzU8   i  = 0;
    
    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    for (i = 0; i < ARRAY_NUM(frame_read_map); i++)
    {
        if (pInfo->FourCC == frame_read_map[i].Fourcc)
        {
            return frame_read_map[i].fnRead_func(pData, pInfo, fDst);
        }
    }

    return ZZ_ERR_UNSUPPORTED;
}



zzStatus ZZ_LoadNext_YV12(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;
    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, pInfo->Pitch - pInfo->Width, SEEK_CUR);
        }
    }

    w     >>= 1;
    h     >>= 1;
    pitch >>= 1;

    // load V
    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY >> 1) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }

    // load U
    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY >> 1) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }


    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_IYUV(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;
  
    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);
  
    w = pInfo->Width;
    h = pInfo->Height;
  
    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;

    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, pInfo->Pitch - pInfo->Width, SEEK_CUR);
        }
    }

    w     >>= 1;
    h     >>= 1;
    pitch >>= 1;

    // load U
    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY >> 1) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }

    // load V
    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY >> 1) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }
      
    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_422H(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;
    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);

        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, pInfo->Pitch - pInfo->Width, SEEK_CUR);
        }
    }

    // if not padding in file, please comment out this line below
    //w >>= 1;
    pitch >>= 1;

    // load U
    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        // Notice: This shift just for vphal's padding.
        ptr += pitch;
        // if not padding in file, please comment out this line below
        //fseek(fSrc, w, SEEK_CUR);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }

    // load V
    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        // Notice: This shift just for vphal's padding.
        ptr += pitch;
        // if not padding in file, please comment out this line below
        //fseek(fSrc, w, SEEK_CUR);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }

    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_422V(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;

    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, pInfo->Pitch - pInfo->Width, SEEK_CUR);
        }
    }

    h >>= 1;
    // load U
    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }

    // load V
    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }

    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_IMC3(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ZZPRINTF("%s imc3 w=%d pitch=%d h=%d\n", __FUNCTION__, w, pitch, h);
    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;

    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        ZZPRINTF("%s imc3 Y read i=%d w=%d pitch=%d h=%d\n", __FUNCTION__, i, w, pitch, h);

        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, pInfo->Pitch - pInfo->Width, SEEK_CUR);
        }
    }

    h >>= 1;
    // if no padding, width = 1/2 original width
    //w >>= 1;
    // load U
    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }

    // load V
    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;

    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }


    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_400P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;

    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, pInfo->Pitch - pInfo->Width, SEEK_CUR);
        }
    }
    
    return ZZ_ERR_NONE;
}


zzStatus ZZ_LoadNext_411P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;

    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, pInfo->Pitch - pInfo->Width, SEEK_CUR);
        }
    }

    //        w >>= 2;
    //        pitch >>= 2;
    // load U
    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }

    // load V
    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }
    

    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_444P(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;

    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, pInfo->Pitch - pInfo->Width, SEEK_CUR);
        }
    }

    // load U
    ptr  = Frame_U(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }

    // load V
    ptr  = Frame_V(pData) + (pInfo->CropX >> 1) + (pInfo->CropY) * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        if(pInfo->Pitch > pInfo->Width)
        {
            fseek(fSrc, (pInfo->Pitch - pInfo->Width)>>1, SEEK_CUR);
        }
    }
    
    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_NV12(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;
    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }

    // load UV
    h     >>= 1;
    ptr = Frame_UV(pData) + pInfo->CropX + (pInfo->CropY >> 1) * pitch;
    for (i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }
    
    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_P010(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;
    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w * 2, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w * 2, ZZ_ERR_MORE_DATA);
    }

    // load UV
    h     >>= 1;
    ptr = Frame_UV(pData) + pInfo->CropX + (pInfo->CropY >> 1) * pitch;
    for (i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w * 2, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w * 2, ZZ_ERR_MORE_DATA);
    }
    
    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_NV12_LINEAR(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;


    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;

    // read luminance plane
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }

    // load UV
    h     >>= 1;
    ptr = Frame_UV(pData) + pInfo->CropX + (pInfo->CropY >> 1) * pitch;
    for (i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
    }

    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_ABGR(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    CHECK_POINTER(Frame_B(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_G(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_R(pData), ZZ_ERR_NOT_INITIALIZED);
    // there is issue with A channel in case of d3d, so A-ch is ignored
    //CHECK_POINTER(pData->A, ZZ_ERR_NOT_INITIALIZED);

    ptr = ZZ_MIN( ZZ_MIN(Frame_R(pData), Frame_G(pData)), Frame_B(pData) );
    ptr = ptr + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, 4*w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, 4*w, ZZ_ERR_MORE_DATA);
    }


    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_ARGB_XRGB(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    CHECK_POINTER(Frame_B(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_G(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_R(pData), ZZ_ERR_NOT_INITIALIZED);
    // there is issue with A channel in case of d3d, so A-ch is ignored
    //CHECK_POINTER(pData->A, ZZ_ERR_NOT_INITIALIZED);

    ptr = ZZ_MIN( ZZ_MIN(Frame_R(pData), Frame_G(pData)), Frame_B(pData) );
    ptr = ptr + pInfo->CropX + pInfo->CropY * pitch;

    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, 4*w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, 4*w, ZZ_ERR_MORE_DATA);
    }

    
    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_R5G6B5(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    CHECK_POINTER(Frame_B(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_G(pData), ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(Frame_R(pData), ZZ_ERR_NOT_INITIALIZED);
    // there is issue with A channel in case of d3d, so A-ch is ignored
    //CHECK_POINTER(pData->A, ZZ_ERR_NOT_INITIALIZED);

    ptr = ZZ_MIN( ZZ_MIN(Frame_R(pData), Frame_G(pData)), Frame_B(pData) );
    ptr = ptr + pInfo->CropX +pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, 2*w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, 2*w, ZZ_ERR_MORE_DATA);
    }

    
    return ZZ_ERR_NONE;
}

zzStatus ZZ_LoadNext_YUY2(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;
    for(i = 0; i < h; i++)
    {
        nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, 2*w, fSrc);
        IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, 2*w, ZZ_ERR_MORE_DATA);
    }


    return ZZ_ERR_NONE;
}
