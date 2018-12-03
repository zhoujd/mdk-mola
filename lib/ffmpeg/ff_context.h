/// ff_context.h

#ifndef INCLUDED_ZZ_FF_CONTEXT
#define INCLUDED_ZZ_FF_CONTEXT 1

#include "utils/context.h"
#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

typedef struct ZZAVContext {
    ZZContext          ctx;

    AVFormatContext    *av_ctx[2];
    AVBufferRef        *hw_device_ctx;
    AVCodecContext     *codec_ctx[2];
    int                video_stream;
    AVStream           *stream[2];

} ZZAVContext;


#endif //INCLUDED_ZZ_FF_CONTEXT
