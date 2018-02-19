#ifndef __TEXTURE2D_SDLBRIDGE__
#define __TEXTURE2D_SDLBRIDGE__
#include "Tech.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <iostream>

class Texture2D_SDL : public GTech2D::Texture2D {

    SDL_Texture* m_pTexture;
public:
    Texture2D_SDL(SDL_Texture* pTexture):m_pTexture(pTexture){}
    ~Texture2D_SDL(){
        if (m_pTexture)
            SDL_DestroyTexture(m_pTexture);
    }
    SDL_Texture* Get(){
        return m_pTexture;
    }

};
#endif /* __TEXTURE2D_SDLBRIDGE__ */