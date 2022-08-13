
#include "SDL2/SDL.h"
#include "Logger.hpp"
#include "BitmapFont.hpp"
#include <array>
#include <array>

class AsciiRendererSDL
{
public: 
    AsciiRendererSDL(SDL_Window* window);
    ~AsciiRendererSDL();
    bool init();
private:
    void loadAsciiTextures();

    Logger logger{"Renderer",false};
    SDL_Renderer* mRenderer;
    SDL_Window* mWindow;
    int mSizex, mSizey;
    int asciiW, asciiH;
    SDL_Rect* charPositions = nullptr;
    std::array<SDL_Texture*,95> charTextures;
};