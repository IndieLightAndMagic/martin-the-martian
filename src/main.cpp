#include "entity.h"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>


const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;
const std::string TAG = "VoidSample";



bool InitImageLoading() {
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
    
    std::cout << TAG << "SDL_Image Version: " << M << "." << m << "." << p << "\n";
    imageFlags = IMG_Init(imageFlags);
    return imageFlags ? true : false;
}

SDL_Surface * pngSurface;

SDL_Surface * LoadSurface (std::string ResourceName) {
    std::string localPath = std::string(RES_DIR) + ResourceName;
    SDL_Surface* pImageSurface = IMG_Load(localPath.c_str());
    return pImageSurface;
}

struct TextureComponent : public  Component {
    SDL_Surface* m_pSurface;
    TextureComponent(std::string textureFileName):
            m_pSurface(LoadSurface(textureFileName)){
        SDL_assert(m_pSurface!=nullptr);
    }

};
class PlayerEntity : public Entity {

public:
    PlayerEntity(std::string textureFileName){

    }
};

int main(int argc, char **argv) {
    
    /* System Start */
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }
    
    SDL_Window* pwin = SDL_CreateWindow("Rendering to a texture!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    SDL_assert(InitImageLoading() == true) ;
    
    /* Load Assets */
    pngSurface = LoadSurface("hero.png");
    SDL_Texture *pngTex = SDL_CreateTextureFromSurface(renderer, pngSurface);
    SDL_FreeSurface(pngSurface);

    //Make a target texture to render too
    Uint32 pixelFormat;
    int access;
    SDL_Rect textureRect;
    {
        auto zero = SDL_QueryTexture(pngTex,&pixelFormat,&access,&textureRect.w, &textureRect.h);
        SDL_assert(zero == 0);
        textureRect.x = 0;
        textureRect.y = 0;

    }

    SDL_Texture *texTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, WIN_WIDTH, WIN_HEIGHT);
    SDL_Rect texRect;
    {
        auto zero = SDL_QueryTexture(texTarget, &pixelFormat, &access, &texRect.w, &texRect.h);
        SDL_assert(zero == 0);
        textureRect.x = (texRect.w - textureRect.w) >> 1;
        textureRect.y = (texRect.h - textureRect.h) >> 1;
    }
    //Now render to the texture
    SDL_SetRenderTarget(renderer, texTarget);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, pngTex, NULL, NULL);
    //Detach the texture
    SDL_SetRenderTarget(renderer, NULL);

    //Now render the texture target to our screen, but upside down
    SDL_RenderClear(renderer);

    //Render in the Center.

    SDL_RenderCopyEx(renderer, texTarget, nullptr, &textureRect, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);

    SDL_Delay(10000);
    SDL_DestroyTexture(texTarget);
    SDL_DestroyTexture(pngTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
