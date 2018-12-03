#ifndef INCLUDED_ZZ_FF_STREAM
#define INCLUDED_ZZ_FF_STREAM 1

#include "utils/stream.h"
#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

typedef struct ZZAVStream {
    ZZStream           base;

    int                video_stream;
    AVStream           *stream[2];

} ZZAVStream;


#endif //INCLUDED_ZZ_FF_STREAM
