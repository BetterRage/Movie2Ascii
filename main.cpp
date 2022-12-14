#include "VideoDecoder.hpp"
#include "AsciiVideoWindow.hpp"
#include "AsciiRenderer.hpp"
#include <thread>

int main(int argc, char **argv)
{
    VideoDecoder vdec(argv[1]);
    vdec.init();
    StreamInfo streamInfo = vdec.getStreamInfo();

    AsciiVideoWindow avwindow("hey", streamInfo.videoWidth, streamInfo.videoHeight);
    avwindow.init();

    AsciiRendererSDL renderer(avwindow.getSDLWindow(), streamInfo.videoWidth, streamInfo.videoHeight, streamInfo.framerate);
    renderer.init();

    size asciisiize = renderer.getAsciiSize();
    vdec.setTargetFrameSize(asciisiize.w, asciisiize.h);

    avwindow.pollEvents();
    renderer.startRendering(std::bind(&VideoDecoder::decode, &vdec, std::placeholders::_1));

    return 0;
}