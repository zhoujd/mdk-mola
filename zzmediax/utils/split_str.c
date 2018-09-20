
#include "split_str.h"


zzStatus ZZSplitStr_ReadLine(zzSplitStrST *pSplitStr)
{
    zzStatus  sts        = ZZ_ERR_NONE;
    zz_char    *pLinePos   = NULL;
    zz_char    *pFieldPos  = NULL;

    static zz_char lineTemp[ZZ_MAX_STR_LEN + 1]  = {0};
    static zz_char tmpStr[ZZ_MAX_STR_LEN + 1] = {0};
    
    CHECK_POINTER(pSplitStr, ZZ_ERR_NOT_INITIALIZED);    
    memset(tmpStr, 0x00, ZZ_MAX_STR_LEN + 1);

    //reset items
    ZERO_MEMORY(pSplitStr->item);
    pSplitStr->count = 0;

    if (zz_strlen(pSplitStr->szData) == 0)
    {
        sts = ZZ_ERR_EOF_STREAM;
        goto END;
    }
    
    pLinePos  = zz_strstr(pSplitStr->szData, pSplitStr->line_sep);
    zz_strncopy(lineTemp, sizeof(lineTemp), pSplitStr->szData, pLinePos - pSplitStr->szData);

    if (zz_strcmp(lineTemp + zz_strlen(lineTemp) - zz_strlen(pSplitStr->field_sep), pSplitStr->field_sep) != 0)
    {
        zz_strcat(lineTemp, sizeof(lineTemp), pSplitStr->field_sep);
    }

    while ((pFieldPos = zz_strstr(lineTemp, pSplitStr->field_sep)) != NULL)
    {
        zz_strncopy(pSplitStr->item[pSplitStr->count].szText, sizeof(pSplitStr->item[pSplitStr->count].szText), lineTemp, pFieldPos - lineTemp);
        pSplitStr->count++;

        zz_strncopy(lineTemp, sizeof(lineTemp), pFieldPos + zz_strlen(pSplitStr->field_sep), sizeof(lineTemp));
    }
    
    zz_strcopy(tmpStr, sizeof(tmpStr), pLinePos + zz_strlen(pSplitStr->line_sep));
    ZERO_MEMORY(pSplitStr->szData);
    zz_strcopy(pSplitStr->szData, sizeof(pSplitStr->szData), tmpStr); 
    
END:
    return sts;
}

zzStatus ZZSplitStr_Set(zzSplitStrST *pSplitStr, zz_char *pData, zz_char *pStrLineSep, zz_char *pStrFieldSep)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSplitStr, ZZ_ERR_NOT_INITIALIZED);
    CHECK_POINTER(pData, ZZ_ERR_NOT_INITIALIZED);

    ZERO_MEMORY(*pSplitStr);


    if (pStrLineSep == NULL)
    {
        zz_strncopy(pSplitStr->line_sep, sizeof(pSplitStr->line_sep), ZZ_DEF_LINE_SEP, sizeof(pSplitStr->line_sep) - 1);
    }
    else
    {
        zz_strncopy(pSplitStr->line_sep, sizeof(pSplitStr->line_sep), pStrLineSep, sizeof(pSplitStr->line_sep) - 1);
    }

    if (pStrFieldSep == NULL)
    {
        zz_strncopy(pSplitStr->field_sep, sizeof(pSplitStr->field_sep), ZZ_DEF_FIELD_SEP, sizeof(pSplitStr->field_sep) - 1);
    }
    else
    {
        zz_strncopy(pSplitStr->field_sep, sizeof(pSplitStr->field_sep), pStrFieldSep, sizeof(pSplitStr->field_sep) - 1);
    }

    if (zz_strcmp(pData + zz_strlen(pData) - zz_strlen(pSplitStr->line_sep), pSplitStr->line_sep) == 0)
    {        
        zz_strncopy(pSplitStr->szData, sizeof(pSplitStr->szData), pData, sizeof(pSplitStr->szData) - 1);
    }
    else
    {
        zz_strncopy(pSplitStr->szData, sizeof(pSplitStr->szData), pData, sizeof(pSplitStr->szData) - 1 - zz_strlen(pStrLineSep));
        zz_strcat(pSplitStr->szData, sizeof(pSplitStr->szData), pStrLineSep); 
    }
    
    return sts;
}

zz_char *  ZZSplitStr_GetIndex(zzSplitStrST *pSplitStr, zzU16 index)
{
    CHECK_POINTER(pSplitStr, NULL);    

    if (index >= pSplitStr->count || index < 0)
    {
        return NULL;
    }

    return pSplitStr->item[index].szText;
 
}

zzU16 ZZSplitStr_GetCount(zzSplitStrST *pSplitStr)
{
    CHECK_POINTER(pSplitStr, 0);    
    return pSplitStr->count;
}
