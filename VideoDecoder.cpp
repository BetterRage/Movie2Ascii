
#include "VideoDecoder.hpp"
#include <assert.h>

VideoDecoder::VideoDecoder(std::string inpath):mInpath(inpath)
{}

VideoDecoder::~VideoDecoder()
{
    avformat_close_input(&av_format_ctx);
    avformat_free_context(av_format_ctx);
    avcodec_free_context(&av_codec_ctx);
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
    
    int video_stream_index = -1;
    AVCodecParameters* avcodecparams;
    const AVCodec* avcodec;

    for(int i=0;i<av_format_ctx->nb_streams;i++)
    {
        auto stream = av_format_ctx->streams[i];
        avcodecparams = av_format_ctx->streams[i]->codecpar;
        avcodec = avcodec_find_decoder(avcodecparams->codec_id);

        if(!avcodec) continue;
        if(avcodecparams->codec_type == AVMEDIA_TYPE_VIDEO){
            mLogger.logInfo("Found video stream");
            video_stream_index = i;
            break;
        }
    }
    
    av_codec_ctx = avcodec_alloc_context3(avcodec);
    avcodec_parameters_to_context(av_codec_ctx,avcodecparams);
    avcodec_open2(av_codec_ctx,avcodec,NULL);
    return true;
}

void VideoDecoder::decode()
{
    AVFrame* avframe = av_frame_alloc();
    AVPacket* avpacket = av_packet_alloc();
    while(av_read_frame(av_format_ctx,avpacket)>=0)
    {
        if(avpacket->stream_index != av_str_idx)
            continue;
        !avcodec_send_packet(av_codec_ctx,avpacket);

        av_packet_unref(avpacket);
        if(!avcodec_receive_frame(av_codec_ctx,avframe))
        {
            printf("decoded fram");
            break;
        }
    }
}

void VideoDecoder::save_image(unsigned char *buf, int wrap, int xsize, int ysize,char *filename)
{
    
}
