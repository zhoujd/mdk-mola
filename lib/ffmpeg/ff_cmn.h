
#ifndef INCLUDED_FF_CMN_H
#define INCLUDED_FF_CMN_H 1

#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

int ffmpeg_decode_init();
int ffmpeg_input_file(const char *filename);

AVFormatContext *GetFormatContext();


int ffmpeg_decode_uninit();

#endif










