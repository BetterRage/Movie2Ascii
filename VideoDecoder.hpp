#include <string>
#include "Logger.hpp"
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

typedef struct streamInfo StreamInfo;
struct streamInfo
{
    float duration;
    float framerate;
    int64_t frames;
    int videoWidth;
    int videoHeight;
    int streamIndex = -1;
};

class VideoDecoder
{
public:
    VideoDecoder(std::string in);
    ~VideoDecoder();
    
    bool init();
    void setTargetFrameSize(int w, int h);

    bool decode(uint8_t* buffer);


    StreamInfo getStreamInfo();
private:
    void setStreamInfo(AVStream* stream);
    Logger mLogger{"Decoder",false};
    std::string mInpath;

    int targetw = -1, targeth = -1;
    AVFrame* curr_frame;
    AVPacket* curr_packet;
    AVFormatContext* av_format_ctx;
    AVCodecContext* av_codec_ctx;
    SwsContext* sw_context;
    StreamInfo info;
};