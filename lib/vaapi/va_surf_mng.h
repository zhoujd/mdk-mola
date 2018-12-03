#ifndef ZZSURFMNG_H
#define ZZSURFMNG_H

#include "cmn.h"
#include "va_utils.h"
#include "frame_cmn.h"
#include "va_ctx_mng.h"
#include "frame_reader.h"
#include "frame_writer.h"

typedef struct _zzSurfaceST zzSurfaceST;

struct _zzSurfaceST
{
    VASurfaceID       id;

    zzVAContextST     *context;

    zzFrameInfo       frameInfo;
    zzFrameData       frameData;

    VAImage           va_image;
    zzBOOL            map_flag;

};

zzStatus ZZSurface_CreateSurface(VADisplay display,  int format, int width, int height, VASurfaceID *pSurfaceID, int surface_cnt);

zzStatus ZZSurface_Create(zzSurfaceST *pSurface);
zzStatus ZZSurface_Release(zzSurfaceST *pSurface);

zzStatus ZZSurface_Lock(zzSurfaceST *pSurface);
zzStatus ZZSurface_UnLock(zzSurfaceST *pSurface);

zzStatus ZZSurface_GetNextInputFrame(zzSurfaceST *pSurface, zzFrameReaderST *pFrameReader);
zzStatus ZZSurface_WriteOutputFrame(zzSurfaceST *pSurface, zzFrameWriterST *pFrameWriter);


#endif //ZZSURFMNG_H
