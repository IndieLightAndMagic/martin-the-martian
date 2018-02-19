#ifndef __SDLBRIDGE_H__
#define __SDLBRIDGE_H__
#include "Tech.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <iostream>

class Tech_SDLBridge : public GTech2D::Tech2D{
private:

    unsigned int m_initFlags{SDL_INIT_EVERYTHING};
    unsigned int m_rendererFlags{SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE};
    SDL_Window* pWindow{nullptr};
    SDL_Renderer* pRenderer{nullptr};

public:
    Tech_SDLBridge(){}
    int Init() override {
        if (SDL_Init(m_initFlags) != 0){
            std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
            return GTech2D::GTECH_ERROR;
        }
        return GTech2D::GTECH_OK;
    }
    int CreateWindow(GTech2D::WindowConfiguration winConfig, unsigned int uiFlags) override {

        pWindow = SDL_CreateWindow(
                winConfig.title.c_str(),
                winConfig.windowRectangle.winPos.x,
                winConfig.windowRectangle.winPos.y,
                winConfig.windowRectangle.winSz.x,
                winConfig.windowRectangle.winSz.y,
                uiFlags);

        if (!pWindow){
            std::cerr << "SDL_CreateWindow failed. \n";
            return GTech2D::GTECH_ERROR;
        }
        return GTech2D::GTECH_OK;

    }
    int CreateRenderer() override {
        pRenderer = SDL_CreateRenderer(pWindow, -1, m_rendererFlags);
        if (!pRenderer){
            std::cerr << "SDL_CreatRenderer failed. \n";
            return GTech2D::GTECH_ERROR;
        }
        return GTech2D::GTECH_OK;
    }
    int Finish() override {

        if (pRenderer) SDL_DestroyRenderer(pRenderer);
        if (pWindow) SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return GTech2D::GTECH_OK;
    }

    std::unique_ptr<GTech2D::GTexture> LoadTexture(std::string) override{
        return std::unique_ptr<GTech2D::GTexture>(nullptr);
    }
    int DestroyTexture(void* vpTexture) override {
        SDL_Texture* pTexture = static_cast<SDL_Texture*>(vpTexture);
    }


    void Assert(bool && exp) override {
        SDL_assert(exp);
    }
    /* The following functions are SDL specific. Should be used ONLY in case. */
    void SetSDLInitFlags(unsigned int uiFlags) {
        m_initFlags = uiFlags;
    }
    void SetSDLRenderFlags(unsigned int uiFlags) {
        m_rendererFlags = uiFlags;
    }
    int InitImageLoading() {
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
        return imageFlags ? GTech2D::GTECH_OK : GTech2D::GTECH_ERROR;
    }
    SDL_Window* GetWindow(){
        return pWindow;
    }
    SDL_Renderer* GetRenderer(){
        return pRenderer;
    }
};
#endif /*__SDLBRIDGE_H__*/