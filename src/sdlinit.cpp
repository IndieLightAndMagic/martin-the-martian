

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

using namespace std;

void SDLInitialization()
{
    constexpr int WindowWidth = 1200;
    constexpr int WindowHeight = 800;
    Uint32 m_initFlags = SDL_INIT_EVERYTHING;
    if (SDL_Init(m_initFlags) != 0){
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        SDL_assert(false);
    }
    auto pWindow = SDL_CreateWindow(
            "Kill the Martian!!!",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WindowWidth,
            WindowHeight,
            SDL_WINDOW_OPENGL
    );
    if (!pWindow){
        std::cerr << "SDL CreateWindow failed. \n";
        SDL_assert(false);
    }

    auto pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
    if (!pRenderer){
        std::cerr << "SDL_CreatRenderer failed. \n";
        SDL_assert(false);
    }

    auto imageFlags = 0;

#ifdef PNG_FORMAT_SHOULD_BE_USED
    imageFlags |= IMG_INIT_PNG;
#endif
#ifdef JPG_FORMAT_SHOULD_BE_USED
    imageFlags |= IMG_INIT_JPG;
#endif
#ifdef TIF_FORMAT_SHOULD_BE_USED
    imageFlags |= IMG_INIT_TIF;
#endif
    //Put your own bmp image here
    const SDL_version* pLinkedVersion = IMG_Linked_Version();
    char M = pLinkedVersion->major; M += 0x30;
    char m = pLinkedVersion->minor; m += 0x30;
    char p = pLinkedVersion->patch; p += 0x30;

    std::cout << "SDL_Image Version: " << M << "." << m << "." << p << "\n";
    imageFlags = IMG_Init(imageFlags);
    if (!imageFlags){
        std::cerr << "IMG_Init failed. \n";
        SDL_assert(false);
    }
}
