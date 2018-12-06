#include "ff_cmn.h"
#include <libavutil/imgutils.h>


static AVFormatContext  *ifmt_ctx = NULL;
static AVBufferRef      *hw_device_ctx = NULL;
static AVCodecContext   *decoder_ctx = NULL;
static int video_stream = -1;
static AVPacket dec_pkt;

static enum AVPixelFormat get_vaapi_format(AVCodecContext *ctx,
                                           const enum AVPixelFormat *pix_fmts)
{
    const enum AVPixelFormat *p;

    for (p = pix_fmts; *p != AV_PIX_FMT_NONE; p++) {
        if (*p == AV_PIX_FMT_VAAPI)
            return *p;
    }

    fprintf(stderr, "Unable to decode this file using VA-API.\n");
    return AV_PIX_FMT_NONE;
}

zzStatus ffmpeg_input_file(const char *filename)
{
    zzStatus   sts    = ZZ_ERR_NONE;
    int ret;
    AVCodec *decoder = NULL;
    AVStream *video = NULL;

    if ((ret = avformat_open_input(&ifmt_ctx, filename, NULL, NULL)) < 0) {
        fprintf(stderr, "Cannot open input file '%s', Error code: %s\n",
                filename, av_err2str(ret));
        return ret;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
        fprintf(stderr, "Cannot find input stream information. Error code: %s\n",
                av_err2str(ret));
        return ret;
    }

    ret = av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decoder, 0);
    if (ret < 0) {
        fprintf(stderr, "Cannot find a video stream in the input file. "
                "Error code: %s\n", av_err2str(ret));
        return ret;
    }
    video_stream = ret;

    if (!(decoder_ctx = avcodec_alloc_context3(decoder)))
        return AVERROR(ENOMEM);

    video = ifmt_ctx->streams[video_stream];
    if ((ret = avcodec_parameters_to_context(decoder_ctx, video->codecpar)) < 0) {
        fprintf(stderr, "avcodec_parameters_to_context error. Error code: %s\n",
                av_err2str(ret));
        return ret;
    }

    decoder_ctx->hw_device_ctx = av_buffer_ref(hw_device_ctx);
    if (!decoder_ctx->hw_device_ctx) {
        fprintf(stderr, "A hardware device reference create failed.\n");
        return AVERROR(ENOMEM);
    }
    decoder_ctx->get_format    = get_vaapi_format;

    if ((ret = avcodec_open2(decoder_ctx, decoder, NULL)) < 0)
    {
        fprintf(stderr, "Failed to open codec for decoding. Error code: %s\n",
                av_err2str(ret));

    }

    return sts;
}


zzStatus ffmpeg_decode_init()
{
    zzStatus   sts    = ZZ_ERR_NONE;
    int ret;

    ret = av_hwdevice_ctx_create(&hw_device_ctx, AV_HWDEVICE_TYPE_VAAPI, NULL, NULL, 0);
    if (ret < 0) {
        fprintf(stderr, "Failed to create a VAAPI device. Error code: %s\n", av_err2str(ret));
        sts = ZZ_ERR_UNKNOWN;
        goto END;
    }

END:
    return sts;
}

zzStatus ffmpeg_decode_uninit()
{
    zzStatus   sts    = ZZ_ERR_NONE;

    avformat_close_input(&ifmt_ctx);
    avcodec_free_context(&decoder_ctx);
    av_buffer_unref(&hw_device_ctx);

    return sts;
}


zzStatus ffmpeg_next_frame(AVFrame *frame)
{
    zzStatus   sts    = ZZ_ERR_NONE;
    int ret;

    if ((ret = av_read_frame(ifmt_ctx, &dec_pkt)) < 0)
    {
        fprintf(stderr, "Error av_read_frame. Error code: %s\n", av_err2str(ret));
        if (ret == AVERROR_EOF) {
            sts = ZZ_ERR_EOF_STREAM;
        } else {
            sts = ZZ_ERR_EOF_STREAM;
        }

        goto END;
    }

    if (video_stream == dec_pkt.stream_index)
    {
        ZZDEBUG("This is zz test video stream -0\n");
        ret = avcodec_send_packet(decoder_ctx, &dec_pkt);
        if (ret < 0) {
            fprintf(stderr, "Error avcodec_send_packet. Error code: %s\n", av_err2str(ret));
            return ret;
        }

        ret = avcodec_receive_frame(decoder_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            sts = ZZ_ERR_EOF_STREAM;
            goto END;
        } else if (ret < 0) {
            fprintf(stderr, "Error avcodec_receive_frame. Error code: %s\n", av_err2str(ret));
            sts = ZZ_ERR_UNKNOWN;
            goto END;
        }

        ZZDEBUG("This is zz test video stream -1\n");


        sts = ffmpeg_dump_frame(frame);
        if (sts != ZZ_ERR_NONE)
        {
            ZZPRINTF("ffmpeg_dump_frame error\n");
            goto END;
        }

    }

END:
    return sts;
}

zzStatus ffmpeg_dump_frame(AVFrame *frame)
{
    zzStatus   sts    = ZZ_ERR_NONE;

    AVFrame *sw_frame = NULL;
    AVFrame *tmp_frame = NULL;
    uint8_t *buffer = NULL;
    int size;
    int ret = 0;

    FILE *output_file = fopen("output/frame.yuv", "a+");

    if (!(sw_frame = av_frame_alloc())) {
        fprintf(stderr, "Can not alloc frame\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }


    fprintf(stderr, "ffmpeg_dump_frame use HW\n");


    /* retrieve data from GPU to CPU */
    if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) < 0) {
        fprintf(stderr, "Error transferring the data to system memory\n");
        goto fail;
    }

    if (frame->format == AV_PIX_FMT_VAAPI) {
        fprintf(stderr, "==hw frame\n");
        /* retrieve data from GPU to CPU */
        if ((ret = av_hwframe_transfer_data(sw_frame, frame, 0)) < 0) {
            fprintf(stderr, "Error transferring the data to system memory\n");
            goto fail;
        }
        tmp_frame = sw_frame;
    } else {
        tmp_frame = frame;
    }

    size = av_image_get_buffer_size(tmp_frame->format, tmp_frame->width,
                                    tmp_frame->height, 1);
    buffer = av_malloc(size);
    if (!buffer) {
        fprintf(stderr, "Can not alloc buffer\n");
        ret = AVERROR(ENOMEM);
        goto fail;
    }
    ret = av_image_copy_to_buffer(buffer, size,
                                  (const uint8_t * const *)tmp_frame->data,
                                  (const int *)tmp_frame->linesize, tmp_frame->format,
                                  tmp_frame->width, tmp_frame->height, 1);
    if (ret < 0) {
        fprintf(stderr, "Can not copy image to buffer\n");
        goto fail;
    }

    if ((ret = fwrite(buffer, 1, size, output_file)) < 0) {
        fprintf(stderr, "Failed to dump raw data.\n");
        goto fail;
    }

fail:
    if (output_file)
        fclose(output_file);

    av_frame_free(&sw_frame);
    av_freep(&buffer);

    return sts;
}
