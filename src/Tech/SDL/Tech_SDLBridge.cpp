#include "Tech/SDL/Tech_SDLBridge.h"


int Tech_SDLBridge::Init() {

    if (SDL_Init(m_initFlags) != 0){
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return GTech::GTECH_ERROR;
    }
    return GTech::GTECH_OK;

}