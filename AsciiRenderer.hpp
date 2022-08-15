#pragma once
#include "SDL2/SDL.h"
#include "Logger.hpp"
#include "BitmapFont.hpp"
#include <array>
#include <functional>

class AsciiRendererSDL
{
public: 
    AsciiRendererSDL(SDL_Window* window,int videoW,int videoH);
    ~AsciiRendererSDL();
    bool init();
    void startRendering(std::function<bool(uint8_t*)> decoder);

private:
    void loadAsciiTextures();
    void loadCharCoordinates();

    Logger logger{"Renderer",false};

    SDL_Renderer* mRenderer;
    SDL_Window* mWindow;

    int windowW, windowH;
    int asciiW, asciiH;
    int videoW,videoH;
    uint8_t* pixelData;
    
    SDL_Rect* charPositions = nullptr;
    std::array<SDL_Texture*,BRIGHTNESS_RESOLUTION> charTextures;
};