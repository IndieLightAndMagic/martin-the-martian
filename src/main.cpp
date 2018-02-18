#include "entity.h"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>


const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;
const std::string TAG = "VoidSample";





SDL_Surface * pngSurface;



class PlayerEntity : public Entity {

public:
    PlayerEntity(std::string textureFileName){

    }
};


class SDLUtils {

    static SDL_Window* pWin;
    static SDL_Renderer* pRenderer;
    static bool bStarted; 
    static SDL_Rect windowDimensions;

    static bool InitImageLoading(int imageFlags) {
    
        //Put your own bmp image here
        const SDL_version* pLinkedVersion = IMG_Linked_Version();
        char M = pLinkedVersion->major; M += 0x30;
        char m = pLinkedVersion->minor; m += 0x30;
        char p = pLinkedVersion->patch; p += 0x30;
        
        std::cout << TAG << "SDL_Image Version: " << M << "." << m << "." << p << "\n";
        imageFlags = IMG_Init(imageFlags);
        return imageFlags ? true : false;
    }

public:
    static bool Init(int width, int height){

        SDL_assert(bStarted == false);
        
        /* System Start */
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
            std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
            return true;
        }
        
        SDLUtils::windowDimensions.w = width;
        SDLUtils::windowDimensions.h = height;
        pWin = SDL_CreateWindow("Rendering to a texture!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
        pRenderer = SDL_CreateRenderer(pWin, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
        
        SDL_assert(InitImageLoading(IMG_INIT_PNG) == true);

        SDLUtils::SetRenderTarget(SDLUtils::WindowTexture());

        bStarted = true;
        return true;
    }

    
    static bool Finish(){

        SDL_assert(bStarted == true);
        
        SDL_DestroyRenderer(pRenderer);
        pRenderer = nullptr;

        SDL_DestroyWindow(pWin);
        pWin = nullptr;

        SDL_Quit();

        bStarted = false;
        return true;

    }
    static SDL_Texture* CreateTextureFromSurface(SDL_Surface* pSurface){ return SDL_CreateTextureFromSurface(pRenderer, pSurface); }

    static SDL_Surface* LoadSurface(std::string ResourceName) {
        
        std::string localPath = std::string(RES_DIR) + ResourceName;
        SDL_Surface* pImageSurface = IMG_Load(localPath.c_str());
        SDL_assert(pImageSurface);
        return pImageSurface;
    
    }

    static SDL_Texture* LoadTexture(std::string ResourceName){

        SDL_Surface* pSurface = SDLUtils::LoadSurface(ResourceName);
        SDL_Texture* pPNGTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
        SDL_FreeSurface(pSurface);
        SDL_assert(pPNGTexture);
        return pPNGTexture;

    }

    static SDL_Texture* WindowTexture(){

        SDL_Texture* pWinTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, windowDimensions.w, windowDimensions.h);
        SDL_assert(pWinTexture);
        return pWinTexture;

    }
    static void SetRenderTarget(SDL_Texture* pTexture){ 
        SDL_assert(SDL_SetRenderTarget(pRenderer, pTexture) == 0);
    }
    static SDL_Rect GetWindowRect(){
        return windowDimensions;
    }
    static void RenderClear(){
        SDL_assert(SDL_RenderClear(pRenderer) == 0);
    }
    static void RenderCopy(SDL_Texture* pTexture, const SDL_Rect* srcRect, const SDL_Rect* dstRect){
        SDL_assert(SDL_RenderCopy(pRenderer, pTexture, srcRect, dstRect) == 0);
    }
    static SDL_Renderer* GetRenderer(){
        SDL_assert(pRenderer != nullptr);
        return pRenderer;
    }
};
SDL_Window* SDLUtils::pWin = nullptr;
SDL_Renderer* SDLUtils::pRenderer = nullptr;
bool SDLUtils::bStarted = false;
SDL_Rect SDLUtils::windowDimensions = {0,0,0,0};

struct TextureComponent : public  Component {
    SDL_Surface* m_pSurface;
    TextureComponent(std::string textureFileName):
            m_pSurface(SDLUtils::LoadSurface(textureFileName)){
        SDL_assert(m_pSurface!=nullptr);
    }

};



int main(int argc, char **argv) {

    /* Init */
    SDLUtils::Init(WIN_WIDTH, WIN_HEIGHT);

    /* Load Assets */
    SDL_Texture* heroTexture = SDLUtils::LoadTexture("hero.png");
    
    //Make a target texture to render too
    Uint32 pixelFormat;
    int access;
    SDL_Rect heroRect;
    {
        auto zero = SDL_QueryTexture(heroTexture, &pixelFormat, &access, &heroRect.w, &heroRect.h);
        SDL_assert(zero == 0);
        heroRect.x = 0;
        heroRect.y = 0;
    }

    SDL_Texture *pTexTarget = SDLUtils::WindowTexture();
    SDL_Rect texRect;
    {
        auto zero = SDL_QueryTexture(pTexTarget, &pixelFormat, &access, &texRect.w, &texRect.h);
        SDL_assert(zero == 0);
        heroRect.x = (texRect.w - heroRect.w) >> 1;
        heroRect.y = (texRect.h - heroRect.h) >> 1;
    }
    
    //Now render to the texture
    SDLUtils::RenderClear();
    SDLUtils::RenderCopy(heroTexture, NULL, NULL);
    
    //Detach the texture
    SDLUtils::SetRenderTarget(NULL);

    //Now render the texture target to our screen, but upside down
    SDLUtils::RenderClear();

    //Render in the Center.

    SDL_RenderCopyEx(SDLUtils::GetRenderer(), pTexTarget, nullptr, &heroRect, 0, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(SDLUtils::GetRenderer());

    SDL_Delay(10000);
    SDL_DestroyTexture(pTexTarget);
    SDL_DestroyTexture(heroTexture);
   
    return 0;
}
