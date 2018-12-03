/// ff_codec.h

#ifndef INCLUDED_ZZ_FF_CODEC
#define INCLUDED_ZZ_FF_CODEC 1

#include "utils/codec.h"
#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

typedef struct ZZAVCodec {
    ZZCodec             base;

    AVCodecContext     *codec_ctx;

} ZZAVCodec;


#endif //INCLUDED_ZZ_FF_CODEC
