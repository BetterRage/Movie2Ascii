
#include "VideoDecoder.hpp"
#include <assert.h>

VideoDecoder::VideoDecoder(std::string inpath)
{
    mInpath = inpath;
}

VideoDecoder::~VideoDecoder()
{
    if (av_format_ctx)
        avformat_free_context(av_format_ctx);
    if (av_codec_ctx)
        avcodec_free_context(&av_codec_ctx);
    if (curr_frame)
        av_frame_free(&curr_frame);
    if (curr_packet)
        av_packet_free(&curr_packet);
    sws_freeContext(sw_context);
}

bool VideoDecoder::init()
{
    av_format_ctx = avformat_alloc_context();
    if (avformat_open_input(&av_format_ctx, mInpath.c_str(), NULL, NULL) != 0)
    {
        avformat_free_context(av_format_ctx);
        mLogger.logError("Failed opening input file");
        return false;
    }
    if (av_format_ctx->nb_streams == 0)
    {
        avformat_free_context(av_format_ctx);
        mLogger.logError("No streams in input file");
        return false;
    }

    AVCodecParameters *avcodecparams;
    const AVCodec *avcodec;

    for (unsigned int i = 0; i < av_format_ctx->nb_streams; i++)
    {
        auto stream = av_format_ctx->streams[i];
        avcodecparams = av_format_ctx->streams[i]->codecpar;
        avcodec = avcodec_find_decoder(avcodecparams->codec_id);

        if (!avcodec)
            continue;
        if (avcodecparams->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            mLogger.logInfo("Found video stream");
            setStreamInfo(stream);
            info.streamIndex = i;
            break;
        }
    }
    av_codec_ctx = avcodec_alloc_context3(avcodec);
    avcodec_parameters_to_context(av_codec_ctx, avcodecparams);
    avcodec_open2(av_codec_ctx, avcodec, NULL);
    return true;
}

void VideoDecoder::setTargetFrameSize(int w, int h)
{
    targetw = w;
    targeth = h;
}

bool VideoDecoder::decode(uint8_t *buffer)
{

    curr_frame = av_frame_alloc();
    curr_packet = av_packet_alloc();
    int result;
    int cont;
    while (true)
    {
        cont = av_read_frame(av_format_ctx, curr_packet);
        if (cont < 0)
            return false;

        if (curr_packet->stream_index != info.streamIndex)
        {
            av_packet_unref(curr_packet);
            continue;
        }

        avcodec_send_packet(av_codec_ctx, curr_packet);
        av_packet_unref(curr_packet);

        // after we send packets pixel format is available so now we can create swscontext
        if (!sw_context)
            sw_context = sws_getContext(info.videoWidth, info.videoHeight, this->av_codec_ctx->pix_fmt,
                                        targetw, targeth, AV_PIX_FMT_YUV420P, SWS_BILINEAR, NULL, NULL, NULL);

        result = avcodec_receive_frame(av_codec_ctx, curr_frame);

        if (result == 0)
        {
            // tmp buffers
            uint8_t data1[targetw * targeth / 2];
            uint8_t data2[targetw * targeth / 2];
            uint8_t *dstdata[4] = {buffer, data1, data2, NULL};
            int dstlinesize[4] = {targetw, targetw / 2, targetw / 2, 0};

            sws_scale(sw_context, curr_frame->data, curr_frame->linesize, 0, curr_frame->height, dstdata, dstlinesize);

            av_frame_unref(curr_frame);
            return true;
        }
        if (result == AVERROR(EOF) || result == AVERROR(EAGAIN))
            continue;
        else if (result != 0)
            return false;
    }
}

StreamInfo VideoDecoder::getStreamInfo()
{
    return info;
}

void VideoDecoder::setStreamInfo(AVStream *stream)
{
    float duration =
        stream->duration *
        (float)stream->time_base.num /
        (float)stream->time_base.den;

    float frameRate = (float)stream->r_frame_rate.num / (float)stream->r_frame_rate.den;

    info = {
        .duration = duration,
        .framerate = frameRate,
        .frames = stream->nb_frames,
        .videoWidth = stream->codecpar->width,
        .videoHeight = stream->codecpar->height};
}