#include "entitymanager.h"

#include "Tech.h"
#include "Tech_SDLBridge.h"
#include "ShipFactory.h"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>


const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const std::string TAG = "VoidSample";


struct System {

};

struct RenderSystem : public System{

    /* To Render a Sprite, a RenderSystem needs:
     * A Sprite Component.
     * A Position Component.
     * A Speed Component.
     * A Scale Component. */


};

namespace GAME {

    ECS::EntityManager  entityManager;
    GTech2D::GTPTech2D   ptech;



    void InitGameTech(){

        ptech = nullptr;
        ptech = GTech2D::Tech2DFactory::StartTechInstance();
        ptech->Init();

        GTech2D::Rectangle2D rectangle2D(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT);
        GTech2D::WindowConfiguration windowConfiguration{
                "Rendering to a texture!",
                rectangle2D
        };

        ptech->CreateWindow(windowConfiguration, 0);
        ptech->CreateRenderer();

        //SDL Specific Code.
        auto sdl_ptech = static_cast<Tech_SDLBridge*>(ptech.get());
        ptech->Assert(sdl_ptech->InitImageLoading() != 0);

    }
}
using namespace GAME;


int main(int argc, char **argv) {

    /* InitGameTech */
    GAME::InitGameTech();

    /* SpawnShip */
    auto ship = GAME::ShipFactory::CreateShip();

    /* Subscribe ship to a rendering system */
    auto ret = GAME::RenderingSystem::SubscribeEntity(ship);

    //auto spHeroEntity = SpriteShipFactory::CreateDefaultShipEntity(ptech);


    GTech2D::Texture2DSize textureSize{WIN_WIDTH, WIN_HEIGHT};
    GTPTexture2D pATexture = ptech->CreateTextureWithSize(textureSize);

    //Now render to the texture
    ptech->SetRenderTarget(pATexture);
    ptech->RenderClear();


    //Make a target texture to render too
    GTech2D::Rectangle2D heroAABB;
    //spHeroTexture->GetSize(heroAABB.winSz);
    heroAABB.winPos.x = (textureSize.w >> 1) -  (heroAABB.winSz.w >> 1);
    heroAABB.winPos.y = (textureSize.h >> 1) -  (heroAABB.winSz.h >> 1);
    //ptech->RenderTexture(spHeroTexture,heroAABB);
    //Detach the texture

    ptech->DetachRenderTexture();
    //SDL_SetRenderTarget(renderer, NULL);

    //Now render the texture target to our screen, but upside down
    ptech->RenderClear();

    //Render in the Center.

    ptech->RenderTextureEx(pATexture, GTech2D::Zero, GTech2D::Zero, 0, pATexture->Center(), GTech2D::FlipType::FLIP_NO);

    //Blit
    ptech->UpdateScreen();

    SDL_Delay(3000);
    ptech->DestroyTexture(pATexture);
    ptech->Finish();
    return 0;
}
