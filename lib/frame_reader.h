/* zz_frame_reader.h ---
 */

#ifndef INCLUDED_ZZ_FRAME_READER_H
#define INCLUDED_ZZ_FRAME_READER_H 1

#include "cmn.h"
#include "utils.h"
#include "file_defs.h"
#include "frame_cmn.h"

typedef struct _zzFrameReaderST
{
    zz_file*      srcFile;
    zz_char       srcFileName[MAX_FILENAME_LEN];

    zzBOOL        demo_flag;

} zzFrameReaderST;

zzStatus ZZFrameReader_Create(zzFrameReaderST **ppRet);
zzStatus ZZFrameReader_Init(zzFrameReaderST *pSelf, zz_char *filename, zzBOOL bDemoFlag);

zzStatus ZZFrameReader_GetNextInputFrame(zzFrameReaderST *pSelf, zzFrameData *pData, zzFrameInfo *pInfo);

zzStatus ZZFrameReader_Release(zzFrameReaderST *pSelf);

#endif /* INCLUDED_ZZ_FRAME_READER_H */
