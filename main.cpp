#include "VideoDecoder.hpp"
#include "AsciiVideoWindow.hpp"
#include <thread>

int main()
{
    VideoDecoder vdec("input.mpeg");
    vdec.init();
    AsciiVideoWindow avwindow("hey",100,100);
    avwindow.init();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}