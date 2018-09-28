/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)raw_read.c
 */

#include "frame_cmn.h"

zzStatus ZZ_LoadNextFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32  w, h, i, pitch;
    zzU32  nBytesRead;
    zzU8  *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);

    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    if(pInfo->FourCC == ZZ_FOURCC_YV12)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_IYUV)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_422H)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_422V)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_IMC3)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_400P)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_411P)
    {
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
    }
    else if(pInfo->FourCC == ZZ_FOURCC_444P)
    {
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
    }
    else if( pInfo->FourCC == ZZ_FOURCC_NV12 )
    {
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
    }
    else if( pInfo->FourCC == ZZ_FOURCC_P010 )
    {
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
    }
    else if( pInfo->FourCC == ZZ_FOURCC_NV12_LINEAR)
    {
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
#if 0
        // load UV
        h     >>= 1;
        w     >>= 1;
        pitch >>= 1;
        ptr = Frame_U(pData) + pInfo->CropX + (pInfo->CropY >> 1) * pitch;
        for (i = 0; i < h; i++)
        {
            nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
            IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        }
        ptr = Frame_V(pData) + pInfo->CropX + (pInfo->CropY >> 1) * pitch;
        for (i = 0; i < h; i++)
        {
            nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, w, fSrc);
            IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, w, ZZ_ERR_MORE_DATA);
        }
#endif
    }
    else if (pInfo->FourCC == ZZ_FOURCC_ABGR)
    {
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
    }
    else if (pInfo->FourCC == ZZ_FOURCC_ARGB || pInfo->FourCC == ZZ_FOURCC_XRGB)
    {
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
    }
    else if ( pInfo->FourCC == ZZ_FOURCC_R5G6B5 )

    {
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
    }
    else if (pInfo->FourCC == ZZ_FOURCC_YUY2)
    {
        ptr = Frame_Y(pData) + pInfo->CropX + pInfo->CropY * pitch;
        for(i = 0; i < h; i++)
        {
            nBytesRead = (zzU32)zz_file_fread(ptr + i * pitch, 1, 2*w, fSrc);
            IOSTREAM_CHECK_NOT_EQUAL(nBytesRead, 2*w, ZZ_ERR_MORE_DATA);
        }
    }
    else
    {
        return ZZ_ERR_UNSUPPORTED;
    }

    return ZZ_ERR_NONE;
}

