
#ifndef INCLUDED_FF_CMN_H
#define INCLUDED_FF_CMN_H 1

#include "cmn.h"

#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>


zzStatus ffmpeg_decode_init();
zzStatus ffmpeg_decode_uninit();
zzStatus ffmpeg_input_file(const char *filename);
zzStatus ffmpeg_next_frame(AVFrame *frame);

#endif
