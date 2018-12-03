/// ff_context.h

#ifndef INCLUDED_ZZ_FF_CONTEXT
#define INCLUDED_ZZ_FF_CONTEXT 1

#include "utils/context.h"
#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

typedef struct ZZAVContext {
    ZZContext          base;

    AVFormatContext    *av_ctx[2];
    AVCodecContext     *codec_ctx[2];

} ZZAVContext;


#endif //INCLUDED_ZZ_FF_CONTEXT
