/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)va_render_parser.c
 */

#include "va_render_parser.h"
#include "split_str.h"

zzStatus render_parser_deinterlace(zzDeinterlaceParamST *pDif, zz_char *pszParams)
{
    zzStatus       sts = ZZ_ERR_NONE;
    zzSplitStrST   splitStr;

    CHECK_POINTER(pDif, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pszParams, ZZ_ERR_NOT_INITIALIZED);

    pDif->base.flag = VP_FILTER_SET;
    pDif->base.type = RENDER_DEINTERLACE_EN;


    ZZSplitStr_Set(&splitStr, pszParams, ":", "=");

    while (ZZ_ERR_NONE == ZZSplitStr_ReadLine(&splitStr))
    {
        zzU16 count = ZZSplitStr_GetCount(&splitStr);
        if (count == 2)
        {
            if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "algo") == 0)
            {
                if (strcmp(ZZSplitStr_GetIndex(&splitStr, 1), "bob") == 0)
                {
                    pDif->type = VAProcDeinterlacingBob;
                }
                else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 1), "adi") == 0)
                {
                    pDif->type = VAProcDeinterlacingMotionAdaptive;
                }
            }
            else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "bff") == 0)
            {
                if (strcmp(ZZSplitStr_GetIndex(&splitStr, 1), "1") == 0)
                {
                    pDif->bff_flag = VP_FILTER_SET;
                }
                else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 1), "0") == 0)
                {
                    pDif->bff_flag = VP_FILTER_UNSET;
                }
            }
            else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "bottom") == 0)
            {
                if (strcmp(ZZSplitStr_GetIndex(&splitStr, 1), "1") == 0)
                {
                    pDif->btm_field_flag = VP_FILTER_SET;
                }
                else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 1), "0") == 0)
                {
                    pDif->btm_field_flag = VP_FILTER_UNSET;
                }
            }
            else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "single") == 0)
            {
                if (strcmp(ZZSplitStr_GetIndex(&splitStr, 1), "1") == 0)
                {
                    pDif->one_field_flag = VP_FILTER_SET;
                }
                else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 1), "0") == 0)
                {
                    pDif->one_field_flag = VP_FILTER_UNSET;
                }
            }
        }
    }

    return sts;

}

zzStatus render_parser_sharpness(zzSharpnessParamST *pSharp, zz_char *pszParams)
{
    zzStatus       sts = ZZ_ERR_NONE;
    zzSplitStrST   splitStr;

    CHECK_POINTER(pSharp, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pszParams, ZZ_ERR_NOT_INITIALIZED);

    pSharp->base.flag = VP_FILTER_SET;
    pSharp->base.type = RENDER_SHARPNESS_EN;

    ZZSplitStr_Set(&splitStr, pszParams, ":", "=");

    while (ZZ_ERR_NONE == ZZSplitStr_ReadLine(&splitStr))
    {
        zzU16 count = ZZSplitStr_GetCount(&splitStr);
        if (count == 2)
        {
            if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "factor") == 0)
            {
                zz_sscanf(ZZSplitStr_GetIndex(&splitStr, 1), ZZ_STRING("%hd"), &pSharp->factor);
            }
        }
    }

    return sts;
}

zzStatus render_parser_amp(zzAmpParamST *pAmp, zz_char *pszParams)
{
    zzStatus       sts = ZZ_ERR_NONE;
    zzSplitStrST   splitStr;

    CHECK_POINTER(pAmp, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pszParams, ZZ_ERR_NOT_INITIALIZED);

    pAmp->base.flag = VP_FILTER_SET;
    pAmp->base.type = RENDER_AMP_EN;

    ZZSplitStr_Set(&splitStr, pszParams, ":", "=");

    while (ZZ_ERR_NONE == ZZSplitStr_ReadLine(&splitStr))
    {
        zzU16 count = ZZSplitStr_GetCount(&splitStr);
        if (count == 2)
        {
            if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "hue") == 0)
            {
                zz_sscanf(ZZSplitStr_GetIndex(&splitStr, 1), ZZ_STRING("%lf"), &pAmp->hue);
            }
            else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "saturation") == 0)
            {
                zz_sscanf(ZZSplitStr_GetIndex(&splitStr, 1), ZZ_STRING("%lf"), &pAmp->saturation);
            }
            else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "brightness") == 0)
            {
                zz_sscanf(ZZSplitStr_GetIndex(&splitStr, 1), ZZ_STRING("%lf"), &pAmp->brightness);
            }
            else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "contrast") == 0)
            {
                zz_sscanf(ZZSplitStr_GetIndex(&splitStr, 1), ZZ_STRING("%lf"), &pAmp->contrast);
            }
        }
    }

    return sts;
}

zzStatus render_parser_denoise(zzDenoiseParamST  *pDN, zz_char *pszParams)
{
    zzStatus       sts = ZZ_ERR_NONE;
    zzSplitStrST   splitStr;

    CHECK_POINTER(pDN, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pszParams, ZZ_ERR_NOT_INITIALIZED);

    pDN->base.flag = VP_FILTER_SET;
    pDN->base.type = RENDER_DEINTERLACE_EN;

    ZZSplitStr_Set(&splitStr, pszParams, ":", "=");

    while (ZZ_ERR_NONE == ZZSplitStr_ReadLine(&splitStr))
    {
        zzU16 count = ZZSplitStr_GetCount(&splitStr);
        if (count == 2)
        {
            if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "factor") == 0)
            {
                zz_sscanf(ZZSplitStr_GetIndex(&splitStr, 1), ZZ_STRING("%hd"), &pDN->factor);
            }
        }
    }

    return sts;
}

zzStatus render_parser_comp(zzCompParamST  *pComp, zz_char *pszParams)
{
    zzStatus       sts = ZZ_ERR_NONE;
    zzSplitStrST   splitStr;

    CHECK_POINTER(pComp, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pszParams, ZZ_ERR_NOT_INITIALIZED);

    pComp->base.flag = VP_FILTER_SET;
    pComp->base.type = RENDER_COMP_EN;

    ZZSplitStr_Set(&splitStr, pszParams, ":", "=");

    while (ZZ_ERR_NONE == ZZSplitStr_ReadLine(&splitStr))
    {
        zzU16 count = ZZSplitStr_GetCount(&splitStr);
        if (count == 2)
        {
            if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "alpha") == 0)
            {
                zz_sscanf(ZZSplitStr_GetIndex(&splitStr, 1), ZZ_STRING("%f"), &pComp->comp_alpha);
            }
            else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "row") == 0)
            {
                zz_sscanf(ZZSplitStr_GetIndex(&splitStr, 1), ZZ_STRING("%hd"), &pComp->row_num);
            }
            else if (strcmp(ZZSplitStr_GetIndex(&splitStr, 0), "col") == 0)
            {
                zz_sscanf(ZZSplitStr_GetIndex(&splitStr, 1), ZZ_STRING("%hd"), &pComp->col_num);
            }
        }
    }

    return sts;
}


zzStatus render_parser_hdr(zzHDRParamST  *pHDR, zz_char *pszParams)
{
    zzStatus       sts = ZZ_ERR_NONE;
    zzSplitStrST   splitStr;

    CHECK_POINTER(pHDR, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pszParams, ZZ_ERR_NOT_INITIALIZED);


    return sts;
}
