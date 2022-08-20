#include "AsciiRenderer.hpp"
#include <thread>
#include <iostream>
#include <cmath>

AsciiRendererSDL::AsciiRendererSDL(SDL_Window *window, int videoW, int videoH, int framerate)
{
    timePerFrame = 1000000.0 / framerate; // time in microseconds

    SDL_GetWindowSize(window, &this->window.w, &this->window.h);
    ascii.w = std::ceil(this->window.w / 8.0f);
    ascii.h = std::ceil(this->window.h / 8.0f);

    this->video.h = videoH;
    this->video.w = videoW;

    mWindow = window;
    charPositions = (SDL_Rect *)malloc(ascii.w * ascii.h * sizeof(SDL_Rect));
}

AsciiRendererSDL::~AsciiRendererSDL()
{
    SDL_DestroyRenderer(mRenderer);
    if (charPositions != nullptr)
        free(charPositions);
}

bool AsciiRendererSDL::init()
{
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);
    if (mRenderer == NULL)
    {
        logger.logError(SDL_GetError());
        return false;
    }

    loadAsciiTextures();
    loadCharCoordinates();
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
    return true;
}

void AsciiRendererSDL::startRendering(std::function<bool(uint8_t *)> decoder)
{
    uint8_t *data = (uint8_t *)malloc(ascii.w * ascii.h);
    while (decoder(data))
    {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
        SDL_RenderClear(mRenderer);
        for (int i = 0; i < ascii.w * ascii.h; i++)
            SDL_RenderCopy(mRenderer, charTextures[data[i] / 8], NULL, charPositions + i);
        SDL_RenderPresent(mRenderer);
        while(std::chrono::system_clock::now()<(tp+std::chrono::microseconds(timePerFrame)));
    }
}

size AsciiRendererSDL::getAsciiSize()
{
    return ascii;
}

void AsciiRendererSDL::loadAsciiTextures()
{
    SDL_Surface *surface = SDL_CreateRGBSurface(0, 8, BYTES_PER_CHARACTER, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

    for (int i = 0; i < BRIGHTNESS_RESOLUTION; i++)
    {
        const std::array<unsigned char, BYTES_PER_CHARACTER> *pxlData = &font8x8_basic[pxlIdx[i]];

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
        charTextures[i] = SDL_CreateTextureFromSurface(mRenderer, surface);
    }
    SDL_FreeSurface(surface);
}

void AsciiRendererSDL::loadCharCoordinates()
{
    for (int i = 0; i < (ascii.h * ascii.w); i++)
    {
        charPositions[i] = {
            .x = (i % ascii.w) * 8,
            .y = (i / ascii.w) * 8,
            .w = 8,
            .h = BYTES_PER_CHARACTER};
    }
}
