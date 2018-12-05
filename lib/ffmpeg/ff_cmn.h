
#ifndef INCLUDED_FF_CMN_H
#define INCLUDED_FF_CMN_H 1

#include <libavutil/hwcontext.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include "vaapi/va_surf_mng.h"

int ffmpeg_decode_init();
int ffmpeg_decode_uninit();
int ffmpeg_input_file(const char *filename);
int ffmpeg_next_frame(zzSurfaceST *dst_surf);

#endif










