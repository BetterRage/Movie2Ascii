#include "AsciiRenderer.hpp"
#include <thread>
#include <iostream>
#include <cmath>

AsciiRendererSDL::AsciiRendererSDL(SDL_Window *window, int videoW, int videoH)
{
    SDL_GetWindowSize(window, &windowW, &windowH);
    asciiW = std::ceil(windowW / 8.0f);
    asciiH = std::ceil(windowH / 8.0f);

    pixelToCharRatioX = (float)asciiW / (float)videoW;
    pixelToCharRatioY = (float)asciiH / (float)videoH;

    this->videoH = videoH;
    this->videoW = videoW;

    mWindow = window;
    charPositions = (SDL_Rect *)malloc(asciiW * asciiH * sizeof(SDL_Rect));
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
    SDL_RenderClear(mRenderer);

    for (int i = 0; i < asciiH * asciiW; i++)
    {
        SDL_RenderCopy(mRenderer, charTextures[i % BRIGHTNESS_RESOLUTION], NULL, charPositions + i);
    }
    SDL_RenderPresent(mRenderer);
    // std::this_thread::sleep_for(std::chrono::seconds(10));
}

void AsciiRendererSDL::startRendering(std::function<bool(uint8_t *)> decoder)
{
    uint8_t *data = (uint8_t *)malloc(videoW * videoH);
    uint8_t *asciiData = (uint8_t *)malloc(asciiW * asciiH);
    while(decoder(data))
    {
        
    }
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
    for (int i = 0; i < (asciiH * asciiW); i++)
    {
        charPositions[i] = {
            .x = (i % asciiW) * 8,
            .y = (i / asciiW) * 8,
            .w = 8,
            .h = BYTES_PER_CHARACTER};
    }
}
