#include "Tech/SDL/Tech_SDLBridge.h"

namespace GTech {
    
    int Tech_SDLBridge::Init() {

        if (SDL_Init(m_initFlags) != 0){
            std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
            return GTech::GTECH_ERROR;
        }
        return GTech::GTECH_OK;

    }

    int Tech_SDLBridge::CreateWindow(GTech::WindowConfiguration winConfig, unsigned int uiFlags) {

        pWindow = SDL_CreateWindow(
            winConfig.title.c_str(),
            winConfig.windowRectangle.winPos.x,
            winConfig.windowRectangle.winPos.y,
            winConfig.windowRectangle.winSz.w,
            winConfig.windowRectangle.winSz.h,
            uiFlags);

        if (!pWindow){
            std::cerr << "SDL_CreateWindow failed. \n";
            return GTech::GTECH_ERROR;
        }

        SDL_GetWindowSize(pWindow, &pWindowRect.w, &pWindowRect.h);
        return GTech::GTECH_OK;

    }

    void Tech_SDLBridge::UpdateEvents() {

        SDL_Event e;
        while(SDL_PollEvent(&e))
        {   
            //Dispatch registered Keyboard Events.
            DispatchKeyboardEvents(e);
        }

    }
}

















