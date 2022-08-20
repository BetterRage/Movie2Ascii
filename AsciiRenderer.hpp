#pragma once
#include "SDL2/SDL.h"
#include "Logger.hpp"
#include "BitmapFont.hpp"
#include <array>
#include <functional>

typedef struct size size;

struct size
{
    int w;
    int h;
};

class AsciiRendererSDL
{
public:
    AsciiRendererSDL(SDL_Window *window, int videoW, int videoH, int framerate);
    ~AsciiRendererSDL();
    bool init();
    void startRendering(std::function<bool(uint8_t *)> decoder);
    size getAsciiSize();

private:
    void loadAsciiTextures();
    void loadCharCoordinates();

    Logger logger{"Renderer", false};

    SDL_Renderer *mRenderer;
    SDL_Window *mWindow;

    size video, window, ascii;

    int timePerFrame;

    SDL_Rect *charPositions = nullptr;
    std::array<SDL_Texture *, BRIGHTNESS_RESOLUTION> charTextures;
};