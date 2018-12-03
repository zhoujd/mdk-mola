#ifndef INCLUDED_ZZ_FF_FRAME
#define INCLUDED_ZZ_FF_FRAME 1

#include "utils/frame.h"
#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>


typedef struct ZZAVFrame {
    ZZFrame  frame;

} ZZAVFrame;


#endif //INCLUDED_ZZ_FF_FRAME
