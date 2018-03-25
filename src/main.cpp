#include "entitymanager.h"

#include "Tech_SDLBridge.h"
#include "ShipFactory.h"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>


const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;



namespace ACTION {

    template <typename T>
    struct Action{
        std::weak_ptr<GTech2D::Tech2DEvent> e;
        void(*fpointer)(std::weak_ptr<GTech2D::Tech2DEvent>, std::weak_ptr<T>) ;
        std::weak_ptr<T> param;
        void operator()(){
            fpointer(e, param);
        }
    };



}


namespace GAME {

    enum class GameResult   {GAME_OK, GAME_FINISHED};
    GTech2D::GTPTech2D  ptech;
    bool bGameIsOn;


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
        auto sdl_ptech = dynamic_cast<Tech_SDLBridge*>(ptech.get());
        ptech->Assert(sdl_ptech->InitImageLoading() != 0);
        ptech->Assert(sdl_ptech->DetectJoysticks() != 0);
    }

    void ProcessSDLEvents(){

        SDL_Event e;
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_KEYDOWN){
                if (e.key.keysym.sym  == SDLK_ESCAPE){

                }
            }
        }
    }
    unsigned int MainLoop() {

        bGameIsOn = true;
        auto sdl_ptech = dynamic_cast<Tech_SDLBridge*>(ptech.get());
        while (bGameIsOn){

            //Get events from loop
            SDL_Event e;
            while(SDL_PollEvent(&e)){

                if (e.type == SDL_KEYDOWN){
                    bGameIsOn = false;
                }
            }

        }
        return 0;
    };

}
using namespace GAME;


int main(int argc, char **argv) {

    /* Init Game Technology */
    InitGameTech();

    /* Create Ship */
    auto ship = GAME::ShipFactory::CreateShip(ptech);
    GAME::ShipFactory::SetShipPosition(ship, WIN_WIDTH>>1, WIN_HEIGHT>>1);
    GAME::RenderingSystem::SubscribeEntity(ship);

    /* Create a texture to draw on */
    GTech2D::Texture2DSize textureSize{WIN_WIDTH, WIN_HEIGHT};
    auto pATexture = ptech->CreateTextureWithSize(textureSize);

    /* Set the texture as the drawing texture */
    ptech->SetRenderTarget(pATexture);
    ptech->RenderClear();

    /* Draw the ship */
    GAME::RenderingSystem::DrawSprites(ptech);

    /* Stop having pATexture as the drawing texture */
    ptech->DetachRenderTexture();

    /* Clear the screen */
    ptech->RenderClear();

    /* Take pATexture and render it into the screen */
    ptech->RenderTextureEx(pATexture, GTech2D::Zero, GTech2D::Zero, 0, pATexture->Center(), GTech2D::FlipType::FLIP_NO);

    /* Ok, show the result */
    ptech->UpdateScreen();

    /* Set a 3 seconds delay */
    MainLoop();

    /* Destroy the texture */
    ptech->DestroyTexture(pATexture);

    /* Finish all the tech system (SDL for this case) */
    ptech->Finish();
    return 0;
}
