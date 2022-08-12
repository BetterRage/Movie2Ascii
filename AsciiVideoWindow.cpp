#include "AsciiVideoWindow.hpp"
#include "BitmapFont.hpp"
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
    renderer = SDL_CreateRenderer(window,-1,SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer,&info);
    mLogger.logInfo(info.name);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_Surface* surf = surfacefromfont(45);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
    SDL_Rect dstrect = {.x=0,.y=0,.w=8,.h=8}; 
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,text,NULL,NULL);
    SDL_RenderPresent(renderer);
}

AsciiVideoWindow::AsciiVideoWindow(std::string name,int videowidth,int videoheight)
{
    mName = name;
    mVideoHeight = videoheight;
    mVideoWidth = videowidth;
}

SDL_Surface* surfacefromfont(int i)
{
    std::array<unsigned char,8> *pxlData = &font8x8_basic[i];
    std::array<uint32_t,64> rgbData;
    SDL_Surface* surf = SDL_CreateRGBSurface(0,8,8,32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
    for(uint i=0;i<pxlData->size();i++)
    {
        for(int j=0;j<8;j++)
        {
            if(pxlData->at(i)&(1<<j))
                ((uint32_t*)(surf->pixels))[i*8+j]=0xFFFFFFFF;
            else
                ((uint32_t*)(surf->pixels))[i*8+j]=0;
        }
    }
    return surf;
}
