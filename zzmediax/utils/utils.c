/* Last modified Time-stamp: <zhoujd 2007-12-30 16:29:58>
 * @(#)utils.c
 */

#include "utils.h"


void*   ZZAllocAndZeroMem(size_t size)
{
    void* ptr;
    
    ptr = malloc(size);
    
    if (ptr != NULL)
    {
        memset(ptr, 0, size);
    }

    return ptr;
}

zzStatus ZZInfoPrint(zz_char*  pStrInfoFile, zz_char* format, ...)
{
    zzStatus sts    = ZZ_ERR_NONE;
    zz_file   *fp     = NULL;
    va_list    argptr;
    int        cnt    = 0;
    
    zz_file_fopen(fp, pStrInfoFile, ZZ_STRING("a"));
    CHECK_POINTER(fp, ZZ_ERR_ABORTED);

    va_start(argptr, format);
    cnt = zz_file_vfprintf(fp, format, argptr);
    va_end(argptr);

    if (cnt < 0)
    {
        sts = ZZ_ERR_UNKNOWN;
        goto END;
    }
    
END:
    if (fp != NULL)
    {
        zz_file_fclose(fp);
    }

    return sts;
}

