#ifndef INCLUDED_ZZ_FF_FORMAT
#define INCLUDED_ZZ_FF_FORMAT 1

#include "utils/format.h"
#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

typedef struct ZZAVFormat {
    ZZFormat           base;

    AVFormatContext    *format_ctx;

} ZZAVFormat;

#endif //INCLUDED_ZZ_FF_FORMAT
