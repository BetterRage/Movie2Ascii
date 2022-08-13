#include "AsciiVideoWindow.hpp"
#include <array>

SDL_Surface* surfacefromfont(int i=1);

void AsciiVideoWindow::init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    ySize = (mVideoHeight * xSize) / mVideoWidth;
    window = SDL_CreateWindow(
        mName.c_str(),
        SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,xSize,ySize,
        SDL_WindowFlags::SDL_WINDOW_SHOWN);
    renderer = std::make_unique<AsciiRendererSDL>(window);    
    renderer->init();
}

AsciiVideoWindow::AsciiVideoWindow(std::string name,int videowidth,int videoheight)
{
    mName = name;
    mVideoHeight = videoheight;
    mVideoWidth = videowidth;

}

AsciiVideoWindow::~AsciiVideoWindow()
{
    SDL_DestroyWindow(window);
}