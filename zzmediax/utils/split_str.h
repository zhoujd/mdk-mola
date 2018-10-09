/* split_str.h
 *
 */

#ifndef ZSPLITSTR_H
#define ZSPLITSTR_H

#include "cmn.h"

#define ZZ_MAX_SPLIT_ITEM  100
#define ZZ_MAX_STR_LEN     256
#define ZZ_MAX_SEP_LEN     10

#define ZZ_DEF_LINE_SEP     ZZ_STRING(":")
#define ZZ_DEF_FIELD_SEP    ZZ_STRING("=")

typedef struct _zzSplitStrST   zzSplitStrST;
typedef struct _zzSplitItemST  zzSplitItemST;


struct _zzSplitItemST
{
    zz_char szText[ZZ_MAX_SPLIT_ITEM + 1];
};

struct _zzSplitStrST
{
    zz_char             szData[ZZ_MAX_STR_LEN + 1];

    zzSplitItemST       item[ZZ_MAX_SPLIT_ITEM];
    zzU16               count;

    zz_char             field_sep[ZZ_MAX_SEP_LEN];
    zz_char             line_sep[ZZ_MAX_SEP_LEN];
};

zzStatus ZZSplitStr_ReadLine(zzSplitStrST *pSplitStr);
zzStatus ZZSplitStr_Set(zzSplitStrST *pSplitStr, zz_char *pData, zz_char *pStrLineSep, zz_char *pStrFieldSep);

zz_char* ZZSplitStr_GetIndex(zzSplitStrST *pSplitStr, zzU16 index);
zzU16    ZZSplitStr_GetCount(zzSplitStrST *pSplitStr);


//============ using like this begin =====================
#if 0 //just for sample, don`t open it
#include "framework/utils/split_str.h"

zzSplitStrST   splitStr;
ZZSplitStr_Set(&splitStr, "ief=1=2=3:factor=2:comp=1=a", ":", "=");

while (ZZ_ERR_NONE == ZZSplitStr_ReadLine(&splitStr))
{
    zzU16 count = ZZSplitStr_GetCount(&splitStr);
    ZZPRINTF("count = %d\n", count);
    
    int i = 0;
    for (i = 0; i < count; i++)
    {
        ZZPRINTF("i = %d, %s\n", i, ZZSplitStr_GetIndex(&splitStr, i));
    }
}

#endif //zhoujd
//============ using like this end =======================


#endif //ZSPLITSTR_H
