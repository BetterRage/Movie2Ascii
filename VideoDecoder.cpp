
#include "VideoDecoder.hpp"
#include <assert.h>

VideoDecoder::VideoDecoder(std::string inpath)
{
    mInpath=inpath;
}

VideoDecoder::~VideoDecoder()
{
    if(av_format_ctx)
        avformat_close_input(&av_format_ctx);      
    if(av_codec_ctx)
        avcodec_free_context(&av_codec_ctx);
    if(curr_frame)
        av_frame_free(&curr_frame);
    if(curr_packet)
        av_packet_free(&curr_packet);
}

bool VideoDecoder::init()
{
    av_format_ctx = avformat_alloc_context();
    if(avformat_open_input(&av_format_ctx,mInpath.c_str(),NULL,NULL)!=0)
    {
        avformat_free_context(av_format_ctx);
        mLogger.logError("Failed opening input file");
        return false;
    }
    if(av_format_ctx->nb_streams == 0)
    {
        avformat_free_context(av_format_ctx);
        mLogger.logError("No streams in input file");
        return false;
    }
    
    AVCodecParameters* avcodecparams;
    const AVCodec* avcodec;

    for(unsigned int i=0;i<av_format_ctx->nb_streams;i++)
    {
        auto stream = av_format_ctx->streams[i];
        avcodecparams = av_format_ctx->streams[i]->codecpar;
        avcodec = avcodec_find_decoder(avcodecparams->codec_id);

        if(!avcodec) continue;
        if(avcodecparams->codec_type == AVMEDIA_TYPE_VIDEO){
            mLogger.logInfo("Found video stream");
            setStreamInfo(stream);
            info.streamIndex = i;
            break;
        }
    }
    
    curr_frame = av_frame_alloc();
    curr_packet = av_packet_alloc();

    av_codec_ctx = avcodec_alloc_context3(avcodec);
    avcodec_parameters_to_context(av_codec_ctx,avcodecparams);
    avcodec_open2(av_codec_ctx,avcodec,NULL);
    return true;
}

bool VideoDecoder::decode(uint8_t* buffer)
{
    curr_frame->data[0] = buffer;
    while(av_read_frame(av_format_ctx,curr_packet)==0)
    {
        if(curr_packet->stream_index != info.streamIndex)
        {
            av_packet_unref(curr_packet);
            continue;
        }
        avcodec_send_packet(av_codec_ctx,curr_packet);
        av_packet_unref(curr_packet);
        if(!avcodec_receive_frame(av_codec_ctx,curr_frame))
        {
            return true;
        }
    }
    return false;
}

StreamInfo VideoDecoder::getStreamInfo()
{
    return info;
}

void VideoDecoder::setStreamInfo(AVStream* stream)
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
        .videoHeight = stream->codecpar->height
    };
}