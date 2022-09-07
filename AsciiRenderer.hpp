#pragma once
#include "SDL2/SDL.h"
#include "Logger.hpp"
#include "BitmapFont.hpp"
#include <array>
#include <atomic>
#include <functional>
#include <readerwriterqueue/atomicops.h>
#include <readerwriterqueue/readerwriterqueue.h>

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
    void renderCharacters(uint8_t *buf);
    void renderRun();
    void decoderRun();

    Logger logger{"Renderer"};

    SDL_Renderer *mRenderer;
    SDL_Window *mWindow;

    size video, window, ascii;

    int timePerFrame;

    std::function<bool(uint8_t *)> decoder;
    std::atomic_bool decoding;

    uint8_t *data1;
    uint8_t *data2;
    uint8_t *currBuffer;
    uint8_t *renderBuffer;

    SDL_Rect *charPositions = nullptr;
    std::array<SDL_Texture *, BRIGHTNESS_RESOLUTION> charTextures;
    moodycamel::ReaderWriterQueue<uint8_t *> framequeue;
    std::thread decode;
};