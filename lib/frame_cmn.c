/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)frame_cmn.c
 */


#include "frame_cmn.h"

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
    else if ( 0 == zz_strcmp(strInput, ZZ_STRING("p010")) )
    {
        *pFourCC = ZZ_FOURCC_P010;
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
    case ZZ_FOURCC_P010:
        ret = "p010";
        break;
    default:
        ret = "unknown format";
        break;
    }

    return ret;
}

zzU8 ZZ_GetPicStruct(zzU8 picStruct )
{
    if ( 0 == picStruct )
    {
        return ZZ_PICSTRUCT_FIELD_TFF;
    }
    else if( 2 == picStruct )
    {
        return ZZ_PICSTRUCT_FIELD_BFF;
    }
    else
    {
        return ZZ_PICSTRUCT_PROGRESSIVE;
    }
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

    ZZPRINTF("Format    = (%s:0x%X)\n", ZZ_FourCC2Str(pFrameInfo->FourCC), pFrameInfo->FourCC);

    ZZPRINTF("Width     = (%d)\n", pFrameInfo->Width);
    ZZPRINTF("Height    = (%d)\n", pFrameInfo->Height);
    ZZPRINTF("CropX     = (%d)\n", pFrameInfo->CropX);
    ZZPRINTF("CropY     = (%d)\n", pFrameInfo->CropY);
    ZZPRINTF("CropW     = (%d)\n", pFrameInfo->CropW);
    ZZPRINTF("CropH     = (%d)\n", pFrameInfo->CropH);
    ZZPRINTF("PicStruct = (%s:%d)\n", ZZ_PicStruct2Str(pFrameInfo->PicStruct), pFrameInfo->PicStruct);

    return sts;
}

zzStatus ZZ_ParseOwnFrameInfo(zzOwnFrameInfoST *frameInfo, int nArgNum, char **strInput)
{
    zzStatus           sts     = ZZ_ERR_NONE;
    zzU8               i       = 1;

    for (i = 1; i < nArgNum; i++ )
    {
        CHECK_POINTER(strInput[i], ZZ_ERR_NULL_PTR);
        {
            if ( 0 == zz_strcmp(strInput[i], ZZ_STRING("-sw")) )
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_IN].Width);
            }
            else if ( 0 == zz_strcmp(strInput[i], ZZ_STRING("-spitch")) )
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_IN].Pitch);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-sh")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_IN].Height);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-scrX")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_IN].CropX);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-scrY")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_IN].CropY);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-scrW")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_IN].CropW);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-scrH")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_IN].CropH);
            }
            else if(0 == zz_strcmp(strInput[i], ZZ_STRING("-spic")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), (short int *)&frameInfo[VPP_IN].PicStruct);
                frameInfo[VPP_IN].PicStruct = ZZ_GetPicStruct(frameInfo[VPP_IN].PicStruct);
            }
            else if(0 == zz_strcmp(strInput[i], ZZ_STRING("-sf")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%lf"), &frameInfo[VPP_IN].dFrameRate);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-dw")) )
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_OUT].Width);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-dh")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_OUT].Height);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-dcrX")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_OUT].CropX);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-dcrY")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_OUT].CropY);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-dcrW")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_OUT].CropW);
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-dcrH")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), &frameInfo[VPP_OUT].CropH);
            }
            else if(0 == zz_strcmp(strInput[i], ZZ_STRING("-dpic")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%hd"), (short int *)&frameInfo[VPP_OUT].PicStruct);
                frameInfo[VPP_OUT].PicStruct = ZZ_GetPicStruct(frameInfo[VPP_OUT].PicStruct);
            }
            else if(0 == zz_strcmp(strInput[i], ZZ_STRING("-df")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                zz_sscanf(strInput[i], ZZ_STRING("%lf"), &frameInfo[VPP_OUT].dFrameRate);
            }

            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-scc")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                sts = ZZ_Str2FourCC(strInput[i], &frameInfo[VPP_IN].FourCC);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("zzmedix unsupport format: %s\n", strInput[i]);
                    goto END;
                }
            }
            else if (0 == zz_strcmp(strInput[i], ZZ_STRING("-dcc")))
            {
                VAL_CHECK(1 + i == nArgNum);
                i++;
                sts = ZZ_Str2FourCC(strInput[i], &frameInfo[VPP_OUT].FourCC);
                if (sts != ZZ_ERR_NONE)
                {
                    ZZPRINTF("zzmediax unsupport format: %s\n", strInput[i]);
                    goto END;
                }
            }

        }
    }

    if (NOT_INIT_VALUE == frameInfo[VPP_IN].CropW)
    {
        frameInfo[VPP_IN].CropW = frameInfo[VPP_IN].Width;
    }

    if (NOT_INIT_VALUE == frameInfo[VPP_IN].CropH)
    {
        frameInfo[VPP_IN].CropH = frameInfo[VPP_IN].Height;
    }

    if (NOT_INIT_VALUE == frameInfo[VPP_OUT].CropW)
    {
        frameInfo[VPP_OUT].CropW = frameInfo[VPP_OUT].Width;
    }

    if (NOT_INIT_VALUE == frameInfo[VPP_OUT].CropH)
    {
        frameInfo[VPP_OUT].CropH = frameInfo[VPP_OUT].Height;
    }

    //picture struct setting
    if (ZZ_PICSTRUCT_UNKNOWN == frameInfo[VPP_IN].PicStruct)
    {
        frameInfo[VPP_IN].PicStruct = ZZ_PICSTRUCT_PROGRESSIVE;
    }

    if (ZZ_PICSTRUCT_UNKNOWN == frameInfo[VPP_OUT].PicStruct)
    {
        frameInfo[VPP_OUT].PicStruct = frameInfo[VPP_IN].PicStruct;
    }
END:
    return sts;
}

zzStatus ZZ_ParamInfo2SurfInfo(zzFrameInfo *pSurfFrameInfo, zzOwnFrameInfoST *pParamFrameInfo)
{
    zzStatus sts    = ZZ_ERR_NONE;

    CHECK_POINTER(pSurfFrameInfo, ZZ_ERR_ABORTED);
    CHECK_POINTER(pParamFrameInfo, ZZ_ERR_ABORTED);

    pSurfFrameInfo->Width     = pParamFrameInfo->Width;
    pSurfFrameInfo->Pitch     = pParamFrameInfo->Pitch;
    pSurfFrameInfo->Height    = pParamFrameInfo->Height;
    pSurfFrameInfo->FourCC    = pParamFrameInfo->FourCC;

    pSurfFrameInfo->CropX     = pParamFrameInfo->CropX;
    pSurfFrameInfo->CropY     = pParamFrameInfo->CropY;
    pSurfFrameInfo->CropW     = pParamFrameInfo->CropW;
    pSurfFrameInfo->CropH     = pParamFrameInfo->CropH;

    pSurfFrameInfo->OutCropX  = pParamFrameInfo->OutCropX;
    pSurfFrameInfo->OutCropY  = pParamFrameInfo->OutCropY;
    pSurfFrameInfo->OutCropW  = pParamFrameInfo->OutCropW;
    pSurfFrameInfo->OutCropH  = pParamFrameInfo->OutCropH;

    pSurfFrameInfo->PicStruct = pParamFrameInfo->PicStruct;

    return sts;
}
