/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)frame_cmn.c
 */


#include "frame_cmn.h"


zzStatus ZZ_LoadNextFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fSrc)
{
    zzU32 w, h, i, pitch;
    zzU32 nBytesRead;
    zzU8 *ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);
    
    w = pInfo->Width;
    h = pInfo->Height;

    pitch = pData->Pitch;

    if(pInfo->MultiSrcWorktaskflowId == 2201)
    {
            pInfo->CropX = 0;
            pInfo->CropY = 0;
    }	 	

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

zzStatus ZZ_WriteFrame(zzFrameData* pData, zzFrameInfo* pInfo, zz_file*  fDst, zzBOOL bWithBox)
{
    zzI32 nBytesRead   = 0;

    zzI32 i, h, w, pitch;
    zzU8* ptr;

    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pInfo, ZZ_ERR_NOT_INITIALIZED);


    if(pInfo->MultiSrcWorktaskflowId == 2201)
    {
	  pInfo->CropX = 0;
	  pInfo->CropY = 0;
    }
    if (pInfo->CropH > 0 && pInfo->CropW > 0 && bWithBox == FALSE) 
    {
	 if(pInfo->MultiSrcWorktaskflowId == 2201)
	 {
		w = pInfo->Width;
              h = pInfo->Height;
	 }
	 else
	 {
             w = pInfo->CropW;
             h = pInfo->CropH;
	 }
    } 
    else 
    {
        w = pInfo->Width;
        h = pInfo->Height;
    }
    
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


zzStatus ZZ_Str2FourCC(zz_char* strInput, zzU32 *pFourCC)
{
    zzStatus  sts    = ZZ_ERR_NONE;

    if ( 0 == zz_strcmp(strInput, ZZ_STRING("yv12")) ) 
    {
        *pFourCC = ZZ_FOURCC_YV12;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("iyuv")) )
    {
        *pFourCC = ZZ_FOURCC_IYUV;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("imc3")) )
    {
        *pFourCC = ZZ_FOURCC_IMC3;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("400p")) )
    {
        *pFourCC = ZZ_FOURCC_400P;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("411p")) )
    {
        *pFourCC = ZZ_FOURCC_411P;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("444p")) )
    {
        *pFourCC = ZZ_FOURCC_444P;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("422h")) )
    {
        *pFourCC = ZZ_FOURCC_422H;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("422v")) )
    {
        *pFourCC = ZZ_FOURCC_422V;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("yuy2")) )
    {
        *pFourCC = ZZ_FOURCC_YUY2;
    } 
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("nv12")) ) 
    {
        *pFourCC = ZZ_FOURCC_NV12;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("nv13")) )
    {
        *pFourCC = ZZ_FOURCC_NV12_LINEAR;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("abgr")) ) 
    {
        *pFourCC = ZZ_FOURCC_ABGR;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("argb")) ) 
    {
        *pFourCC = ZZ_FOURCC_ARGB;
    }    
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("r5g6b5")) ) 
    {
        *pFourCC = ZZ_FOURCC_R5G6B5;
    }
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("xrgb")) ) 
    {
        *pFourCC = ZZ_FOURCC_XRGB;
    }
    else
    {
        sts = ZZ_ERR_UNKNOWN;
        goto END;
    }

END:
    return sts;
}


zz_char* ZZ_FourCC2Str(zzU32 fourcc)
{
    zz_char*  ret = NULL;

    switch (fourcc)
    {
    case ZZ_FOURCC_NV12:
        ret = "nv12";
        break;
    case ZZ_FOURCC_NV12_LINEAR:
        ret = "nv13";
        break;
    case ZZ_FOURCC_YV12:
        ret = "yv12";
        break;
    case ZZ_FOURCC_IMC3:
        ret = "imc3";
        break;
    case ZZ_FOURCC_400P:
        ret = "400p";
        break;
    case ZZ_FOURCC_411P:
        ret = "411p";
        break;
    case ZZ_FOURCC_444P:
        ret = "444p";
        break;
    case ZZ_FOURCC_IYUV:
        ret = "iyuv";
        break;
    case ZZ_FOURCC_422H:
        ret = "422h";
        break;
    case ZZ_FOURCC_422V:
        ret = "422v";
        break;
    case ZZ_FOURCC_YUY2:
        ret = "yuy2";
        break;
    case ZZ_FOURCC_ARGB:
        ret = "argb";
        break;
    case ZZ_FOURCC_ABGR:
        ret = "abgr";
        break;
    case ZZ_FOURCC_R5G6B5:
        ret = "r5g6b5";
        break;
    case ZZ_FOURCC_XRGB:
        ret = "xrgb";
        break;
    default:
        ret = "unknown format";
        break;
    }

    return ret;
}

zz_char* ZZ_PicStruct2Str(zzU8 picStruct)
{
    zz_char*  ret = NULL;
    
    switch (picStruct ) 
    {
    case ZZ_PICSTRUCT_FIELD_TFF:
        ret = "tff";
        break;
    case ZZ_PICSTRUCT_FIELD_BFF:
        ret = "bff";
        break;
    case ZZ_PICSTRUCT_PROGRESSIVE:
        ret = "progressive";
        break;
    default:
        ret = "unknown";
        break;
    }

    return ret;
}


zzStatus ZZ_DumpFrameInfo(zzFrameInfo *pFrameInfo)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pFrameInfo, ZZ_ERR_NULL_PTR);

    ZZPRINTF("Format    = (%s:%d)\n", ZZ_FourCC2Str(pFrameInfo->FourCC), pFrameInfo->FourCC);
    
    ZZPRINTF("Width     = (%d)\n", pFrameInfo->Width);
    ZZPRINTF("Height    = (%d)\n", pFrameInfo->Height);
    ZZPRINTF("CropX     = (%d)\n", pFrameInfo->CropX);
    ZZPRINTF("CropY     = (%d)\n", pFrameInfo->CropY);
    ZZPRINTF("CropW     = (%d)\n", pFrameInfo->CropW);
    ZZPRINTF("CropH     = (%d)\n", pFrameInfo->CropH);
    ZZPRINTF("PicStruct = (%s:%d)\n", ZZ_PicStruct2Str(pFrameInfo->PicStruct), pFrameInfo->PicStruct);
    
    return sts;
}
