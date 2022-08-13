#include "SDL2/SDL.h"
#include <string>
#include "Logger.hpp"
#include "AsciiRenderer.hpp"
#include <memory>

class AsciiVideoWindow
{
public:

    AsciiVideoWindow(std::string name,int videowidth,int videoheight);
    ~AsciiVideoWindow();
    void init();
private:
    const int xSize = 1000;
    int ySize;
    int mVideoHeight;
    int mVideoWidth;
    std::string mName;
    
    Logger mLogger{"Window",false};

    SDL_Window* window;
    std::unique_ptr<AsciiRendererSDL> renderer; 
};