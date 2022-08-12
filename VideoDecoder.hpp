#include <string>
#include "Logger.hpp"
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

class VideoDecoder
{
public:
    VideoDecoder(std::string in);
    ~VideoDecoder();
    bool init();
    void decode();
    void save_image(unsigned char *buf, int wrap, int xsize, int ysize,char *filename);
    
private:
    Logger mLogger{"Decoder",false};
    std::string mInpath;
    AVFormatContext* av_format_ctx;
    AVCodecContext* av_codec_ctx;
    int av_str_idx = -1;
};