#ifndef INCLUDED_ZZ_FF_BUFFER
#define INCLUDED_ZZ_FF_BUFFER 1

#include "utils/buffer.h"
#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

typedef struct ZZAVBufferRef {
    ZZBufferRef  base;

    AVBufferRef  *hw_device_ctx;

} ZZAVBufferRef;


#endif //INCLUDED_ZZ_FF_BUFFER
