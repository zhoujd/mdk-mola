/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)frame_reader.c
 */

#include "frame_reader.h"

zzStatus ZZFrameReader_Create(zzFrameReaderST **ppRet)
{
    zzStatus sts = ZZ_ERR_NONE;
    
    *ppRet = (zzFrameReaderST *)AllocAndZeroMem(sizeof(zzFrameReaderST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }
    memset(*ppRet, 0, sizeof(zzFrameReaderST));

END:
    return sts;
}

zzStatus ZZFrameReader_Init(zzFrameReaderST *pSelf, zz_char *strFileName)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(strFileName, ZZ_ERR_NULL_PTR);

    zz_file_fopen(pSelf->srcFile, strFileName, ZZ_STRING("rb"));
    CHECK_POINTER(pSelf->srcFile, ZZ_ERR_ABORTED);

    //copy file name
    strncpy(pSelf->srcFileName, strFileName, MAX_FILENAME_LEN);
    
    return sts;
}

zzStatus ZZFrameReader_GetNextInputFrame(zzFrameReaderST *pSelf, zzFrameData *pData, zzFrameInfo *pInfo)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZ_LoadNextFrame(pData, pInfo, pSelf->srcFile);
    if (sts != ZZ_ERR_NONE)
    {
        if (zz_file_feof(pSelf->srcFile))
        {
            sts = ZZ_ERR_EOF_STREAM;
            ZZPRINTF("Read end of file:%s\n", pSelf->srcFileName);
        }
        else
        {
            ZZPRINTF("LoadNextFrame error\n");
            goto END;
        }

    }

    
END:    
    return sts;
}

zzStatus ZZFrameReader_Release(zzFrameReaderST *pSelf)
{
    zzStatus sts = ZZ_ERR_NONE;

    //free resource
    if (pSelf->srcFile != NULL)
    {
        zz_file_fclose(pSelf->srcFile);
        pSelf->srcFile = NULL;
    }

    return sts;

}
