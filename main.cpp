#include "VideoDecoder.hpp"
#include "AsciiVideoWindow.hpp"
#include "AsciiRenderer.hpp"
#include <thread>

int main()
{
    VideoDecoder vdec("Big_Buck_Bunny_360_10s_2MB.mp4");
    vdec.init();
    StreamInfo streamInfo = vdec.getStreamInfo();

    AsciiVideoWindow avwindow("hey",streamInfo.videoWidth,streamInfo.videoHeight);
    avwindow.init();
    
    AsciiRendererSDL renderer(avwindow.getSDLWindow(),streamInfo.videoWidth,streamInfo.videoHeight);
    renderer.init();
    renderer.startRendering(std::bind(&VideoDecoder::decode,&vdec,std::placeholders::_1));


    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}