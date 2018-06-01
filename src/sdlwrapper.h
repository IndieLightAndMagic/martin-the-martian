#ifndef __SDLWRAPPER_H__
#define __SDLWRAPPER_H__

#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <utility>
#include <tuple>


namespace GTech  {

    void SDLInitialization();
    void SDLQuit();


    SDL_Texture* SDLCreateTexture(SDL_Rect& rSize);
    SDL_Texture* SDLCreateTextureFromFile(const char* path);
    std::tuple<uint32_t, int , glm::ivec2> SDLQueryTexture(SDL_Texture*);
    SDL_Texture* SDLCreateTextureFromFile(const char *path);

    void SDLDetachRenderTexture();
    void SDLRenderClear();
    void SDLRenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
    void SDLSetRenderTarget(SDL_Texture* pSDLTexture);
    void SDLUpdateScreen();
    std::pair<int, int> SDLWindowSize();

}



#endif //__SDLWRAPPER_H__