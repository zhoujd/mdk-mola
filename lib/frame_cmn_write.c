/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)raw_write.c
 */

#include "frame_cmn.h"

zzStatus ZZ_WriteFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst)
{
    zzI32  nBytesRead   = 0;

    zzI32  i, h, w, pitch;
    zzU8*  ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;
    pitch = pData->Pitch;

    if(pInfo->FourCC == ZZ_FOURCC_YV12)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_IYUV)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_400P)
    {
        ptr   = Frame_Y(pData) + (pInfo->CropX ) + (pInfo->CropY ) * pitch;

        for (i = 0; i < h; i++)
        {
            CHECK_NOT_EQUAL(zz_file_fwrite(ptr+ i * pitch, 1, w, fDst), w, ZZ_ERR_UNDEFINED_BEHAVIOR);
        }
    }
    else if(pInfo->FourCC == ZZ_FOURCC_411P)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_444P)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_IMC3)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_422H)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_422V)
    {

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
    }
    else if( pInfo->FourCC == ZZ_FOURCC_NV12 )
    {

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
    }
    else if( pInfo->FourCC == ZZ_FOURCC_P010 )
    {
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
    }
    else if( pInfo->FourCC == ZZ_FOURCC_YUY2 )
    {
        ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;
        for(i = 0; i < h; i++)
        {
            CHECK_NOT_EQUAL( zz_file_fwrite(ptr+ i * pitch, 1, 2*w, fDst), 2*w, ZZ_ERR_UNDEFINED_BEHAVIOR);
        }
    }
    else if (pInfo->FourCC == ZZ_FOURCC_ARGB || pInfo->FourCC == ZZ_FOURCC_XRGB)
    {
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
    }
    else if (pInfo->FourCC == ZZ_FOURCC_ABGR)
    {
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
    }
    else if (pInfo->FourCC == ZZ_FOURCC_R5G6B5)
    {
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
    }
    else
    {
        return ZZ_ERR_UNSUPPORTED;
    }

    return ZZ_ERR_NONE;
}
