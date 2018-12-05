#include "ff_cmn.h"

static AVFormatContext  *ifmt_ctx = NULL;
static AVBufferRef      *hw_device_ctx = NULL;
static AVCodecContext   *decoder_ctx = NULL;
static int video_stream = -1;


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
    AVPacket dec_pkt;
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
    }

END:
    return sts;
}
