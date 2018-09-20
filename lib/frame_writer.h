/* zz_frame_writer.h --- 
 */

#ifndef INCLUDED_ZZ_FRAME_WRITER_H
#define INCLUDED_ZZ_FRAME_WRITER_H 1


#include "cmn.h"
#include "utils.h"
#include "file_defs.h"
#include "frame_cmn.h"

typedef struct _zzFrameWriteST
{
    zz_file*      dstFile;
    zz_char       dstFileName[MAX_FILENAME_LEN];
    
} zzFrameWriterST;

zzStatus ZZFrameWriter_Create(zzFrameWriterST **ppRet);
zzStatus ZZFrameWriter_Init(zzFrameWriterST *pSelf, zz_char *filename);

zzStatus ZZFrameWriter_WriteFrame(zzFrameWriterST *pSelf, zzFrameData *pData, zzFrameInfo *pInfo, zzBOOL bWithBox);

zzStatus ZZFrameWriter_Release(zzFrameWriterST *pSelf);



#endif /* INCLUDED_ZZ_FRAME_WRITER_H */

