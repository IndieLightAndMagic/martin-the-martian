#ifndef __TEXTURE2D_SDLBRIDGE__
#define __TEXTURE2D_SDLBRIDGE__
#include "Tech/G/Tech.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <iostream>

class Texture2D_SDL : public GTech::Texture2D {

    SDL_Texture* m_pTexture;
public:
    Texture2D_SDL(SDL_Texture* pTexture):m_pTexture(pTexture){
        std::cerr << "Texture2D_SDL ctor: THIS: 0x" << std::hex << this << std::dec << " : ";
        std::cerr << "Texture2D_SDL ctor: SDL_DestroyTexture 0x" << std::hex << m_pTexture << std::dec << "\n";

    }

    ~Texture2D_SDL() override {
        if (m_pTexture){
            SDL_DestroyTexture(m_pTexture);
            std::cerr << "Texture2D_SDL dtor: THIS: 0x" << std::hex << this << std::dec << " : ";
            std::cerr << "Texture2D_SDL dtor: SDL_DestroyTexture 0x" << std::hex << m_pTexture << std::dec << "\n";
            m_pTexture = nullptr;

        }
    }

    int GetSize(GTech::Texture2DSize& rSize) override {


        if (!m_pTexture) return GTech::GTECH_ERROR;

        struct {
            Uint32 pixelFormat;
            int access;
        }textureFormat;
        auto zero = SDL_QueryTexture(m_pTexture, &textureFormat.pixelFormat, &textureFormat.access, &rSize.w, &rSize.h);
        SDL_assert(zero == 0);

        return GTech::GTECH_OK;

    }


    /*** SDL Specific ***/
    SDL_Texture* Get() const{
        return m_pTexture;
    }

};
#endif /* __TEXTURE2D_SDLBRIDGE__ */