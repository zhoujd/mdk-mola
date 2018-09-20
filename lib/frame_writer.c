/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)frame_writer.c
 */

#include "frame_writer.h"


zzStatus ZZFrameWriter_Create(zzFrameWriterST **ppRet)
{
    zzStatus sts = ZZ_ERR_NONE;
    
    *ppRet = (zzFrameWriterST *)AllocAndZeroMem(sizeof(zzFrameWriterST));
    if (*ppRet == NULL)
    {
        ZZPRINTF("==zztest (%s) memory alloc\n", __FUNCTION__);
        sts = ZZ_ERR_MEMORY_ALLOC;
        goto END;
    }
    memset(*ppRet, 0, sizeof(zzFrameWriterST));

END:
    return sts;
    
}

zzStatus ZZFrameWriter_Init(zzFrameWriterST *pSelf, zz_char *strFileName)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(strFileName, ZZ_ERR_NULL_PTR);

    zz_file_fopen(pSelf->dstFile, strFileName, ZZ_STRING("wb"));
    CHECK_POINTER(pSelf->dstFile, ZZ_ERR_ABORTED);

    //copy file name
    strncpy(pSelf->dstFileName, strFileName, MAX_FILENAME_LEN);
    
    return sts;
    
}

zzStatus ZZFrameWriter_WriteFrame(zzFrameWriterST *pSelf, zzFrameData *pData, zzFrameInfo *pInfo, zzBOOL bWithBox)
{
    zzStatus sts = ZZ_ERR_NONE;

    CHECK_POINTER(pSelf, ZZ_ERR_NULL_PTR);

    sts = ZZ_WriteFrame(pData, pInfo, pSelf->dstFile, bWithBox);
    if (sts != ZZ_ERR_NONE)
    {
        ZZPRINTF("WriteFrame error\n");
        goto END;
    }

    
END:    
    return sts;

}

zzStatus ZZFrameWriter_Release(zzFrameWriterST *pSelf)
{
    zzStatus sts = ZZ_ERR_NONE;

    //free resource
    if (pSelf->dstFile != NULL)
    {
        zz_file_fclose(pSelf->dstFile);
        pSelf->dstFile = NULL;
    }

    return sts;
    
}
