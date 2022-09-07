#include "AsciiVideoWindow.hpp"
#include <array>

SDL_Surface *surfacefromfont(int i = 1);

void AsciiVideoWindow::init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    ySize = (mVideoHeight * xSize) / mVideoWidth;
    window = SDL_CreateWindow(
        mName.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xSize, ySize,
        SDL_WindowFlags::SDL_WINDOW_SHOWN);
}

SDL_Window *AsciiVideoWindow::getSDLWindow()
{
    return window;
}

AsciiVideoWindow::AsciiVideoWindow(std::string name, int videowidth, int videoheight)
{
    mName = name;
    mVideoHeight = videoheight;
    mVideoWidth = videowidth;
}

void AsciiVideoWindow::pollEvents()
{
    eventListener = std::thread([]
                                {
        while(true)
        {
            SDL_Event event;
            if(SDL_PollEvent(&event)==1)
            {
                switch (event.type)
                {
                case SDL_WINDOWEVENT:
                    switch (event.window.event)
                    {
                    case SDL_WINDOWEVENT_CLOSE:
                        exit(0);
                        break;
                    break;
                
                default:
                    break;
                }
                }
            }

        } });
}

AsciiVideoWindow::~AsciiVideoWindow()
{
    SDL_DestroyWindow(window);
}