#include "AsciiRenderer.hpp"
#include <thread>

AsciiRendererSDL::AsciiRendererSDL(SDL_Window *window)
{
    asciiW = mSizex / 8;
    asciiH = mSizey / 8;
    mWindow = window;
    charPositions = (SDL_Rect *)malloc(asciiW * asciiH * sizeof(SDL_Rect));
}

AsciiRendererSDL::~AsciiRendererSDL()
{
    SDL_DestroyRenderer(mRenderer);
    if(charPositions!=nullptr)
        free(charPositions);
}

bool AsciiRendererSDL::init()
{
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
    if (mRenderer == NULL)
        logger.logError(SDL_GetError());
    loadAsciiTextures();
    SDL_SetRenderDrawColor(mRenderer,0,0,0,255);
    for(int i=0;i<95;i++)
    {
        SDL_RenderClear(mRenderer);
        SDL_RenderCopy(mRenderer,charTextures[i],NULL,NULL);
        SDL_RenderPresent(mRenderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
    }
}

void AsciiRendererSDL::loadAsciiTextures()
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 8, 8, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

    for(int i=0;i<95;i++)
    {
        const std::array<unsigned char, BYTES_PER_CHARACTER> *pxlData = &font8x8_basic[i];

        for (uint j = 0; j < BYTES_PER_CHARACTER; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                if ((*pxlData)[j] & (1 << k))
                    ((uint32_t *)(surface->pixels))[j * 8 + k] = 0xFFFFFFFF;
                else
                    ((uint32_t *)(surface->pixels))[j * 8 + k] = 0;
            }
        }
        charTextures[i]=SDL_CreateTextureFromSurface(mRenderer,surface); 
    }
    sdlfre
}

