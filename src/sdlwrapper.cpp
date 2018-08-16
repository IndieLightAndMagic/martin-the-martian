// #include "sdlwrapper.h"
#include <sdlwrapper.h>

#include <iostream>
#include <utility>
#include <tuple>

#include <SDL2/SDL.h>
// #include <SDL2_image/SDL_image.h>
#include <SDL2/SDL_image.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using namespace std;


SDL_Window* pWindow;
SDL_Renderer* pRenderer;


namespace GTech {

    SDL_Texture* SDLCreateTexture(SDL_Rect& rSize) {
        return SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rSize.w, rSize.h);
    }

    void SDLInitialization() {
        constexpr int WindowWidth = 1200;
        constexpr int WindowHeight = 800;

        Uint32 m_initFlags = SDL_INIT_EVERYTHING;
        if (SDL_Init(m_initFlags) != 0){
            std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
            SDL_assert(false);
        }
        pWindow = nullptr;
        pWindow = SDL_CreateWindow(
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
        pRenderer = nullptr;
        pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE);
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

    SDL_Texture* SDLCreateTextureFromSurface(SDL_Surface* pSurface) {
        auto pSDLTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
        SDL_FreeSurface(pSurface);
        if (!pSDLTexture){
            std::cerr << "Tech_SDLBridge: Couldn't create a texture... \n";
            SDL_assert(false);
        }
        return pSDLTexture;
    }

    SDL_Texture *SDLCreateTextureFromFile(const char *path) {
        auto pSDLTexture = IMG_LoadTexture(pRenderer, path);
        if (!pSDLTexture){
            std::cerr << "Tech_SDLBridge: Couldn't create a texture... \n";
            SDL_assert(false);
        }
        return pSDLTexture;
    }

    void SDLDetachRenderTexture() {
        SDL_assert(SDL_SetRenderTarget(pRenderer, nullptr) == 0);
    }

    std::tuple<uint32_t, int , glm::ivec2> SDLQueryTexture(SDL_Texture* pTexture){

        uint32_t format;
        int access, w, h;
        SDL_QueryTexture(pTexture, &format, &access, &w, &h);
        return std::tuple<uint32_t, int , glm::ivec2>{format, access, glm::ivec2(w, h)};

    };
    void SDLSetRenderDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        SDL_SetRenderDrawColor(pRenderer, r, g, b, a);
    }
    void SDLRenderClear(void) {
        SDL_assert(SDL_RenderClear(pRenderer) == 0);
    }
    void SDLRenderCopyEx(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect, const double angle, glm::vec3* anchorPoint) {
        auto apx = static_cast<int>(anchorPoint->x * dstrect->w);
        auto apy = static_cast<int>(anchorPoint->y * dstrect->h);
        SDL_Point anchorPointSDL{apx, apy};
        auto dangle = fmod(angle, 360.0f);
        SDL_assert(SDL_RenderCopyEx(pRenderer, texture, srcrect, dstrect, dangle, &anchorPointSDL, SDL_FLIP_NONE) == 0);
    }
    void SDLRenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect) {
        SDL_assert(SDL_RenderCopy(pRenderer, texture, srcrect, dstrect) == 0);
    }
    void SDLSetRenderTarget(SDL_Texture* pSDLTexture) {
        SDL_assert(SDL_SetRenderTarget(pRenderer, pSDLTexture) == 0);
    }
    void SDLUpdateScreen() {
        SDL_RenderPresent(pRenderer);
    }
    void SDLWindowSize(int* w, int* h) {
        SDL_GetWindowSize(pWindow, w, h);
    }

    pair<int, int> SDLWindowSize(){

        int w,h;
        SDL_GetWindowSize(pWindow, &w, &h);
        auto p = make_pair(w,h);
        return p;

    }

    void SDLQuit()
    {
        SDL_Quit();
    }
}
