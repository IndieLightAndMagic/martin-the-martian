#include "entitymanager.h"

#include "Tech/SDL/Tech_SDLBridge.h"
#include "ShipFactory.h"
#include "SignalSlot/signalslot.h"
#include "Event/eventkeyboard.h"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>


const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;


namespace GAME {

    enum class GameResult   {GAME_OK, GAME_FINISHED};
    GTech::Tech  ptech;

    bool bGameIsOn;

    enum class MovingState { STAND_STILL = -1,  GOING_RIGHT = 0, GOING_LEFT = 1, DODGE_RIGHT = 2, DODGE_LEFT = 3};
    MovingState movingState;

    void OnEscPressed(const GTech::KBEvent&, const GTech::KBKey&);

    void OnArrowKeyPressed(const GTech::KBEvent&, const GTech::KBKey&);

    void InitGameTech(){

        ptech = nullptr;
        ptech = GTech::Tech2DFactory::StartTechInstance();
        ptech->Init();
        

        GTech::Rectangle2D rectangle2D(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT);
        GTech::WindowConfiguration windowConfiguration{
                "Rendering to a texture!",
                rectangle2D
        };

        ptech->CreateWindow(windowConfiguration, 0);
        ptech->CreateRenderer();

        //SDL Specific Code.
        auto sdl_ptech = dynamic_cast<Tech_SDLBridge*>(ptech.get());
        ptech->Assert(sdl_ptech->InitImageLoading() != 0);
        //ptech->Assert(sdl_ptech->DetectJoysticks() != 0);



    }
    namespace SCENE{
        void Init()
        {
            /* Connect signals to Scene slots */
            auto arrowKeys = std::vector<const GTech::KBKey>{K_RIGHT, K_LEFT, K_UP, K_DOWN};
            ptech->RegisterKeyboardEvent(GTech::KBEvent::KEY_PRESSED, GTech::KBKey::K_ESC, GAME::OnEscPressed);
            ptech->RegisterKeyboardEvent(GTech::KBEvent::KEY_PRESSED, arrowKeys, GAME::OnArrowKeyPressed);
        }
    }
    
    unsigned int MainLoop() {
 
        bGameIsOn = true;
        auto sdl_ptech = dynamic_cast<Tech_SDLBridge*>(ptech.get());
        while (bGameIsOn){
            sdl_ptech->UpdateEvents();
            ECS::RenderingSystem::DrawSprites(ptech);

            ECS::RenderingSystem::UpdateRenderingSystem(ptech);
        }
        return 0;
    }
    void OnEscPressed(const GTech::KBEvent& kbEvent, const GTech::KBKey & kbKey){
        std::cout << "GAME::OnEscPressed "  << __FUNCTION__ << std::endl;
        bGameIsOn = false;
    }
    void OnArrowKeyPressed(const GTech::KBEvent& kbEvent, const GTech::KBKey & kbKey){


        if (kbKey ==  KBKey::K_LEFT){
            movingState = MovingState::GOING_LEFT;
            std::cout << __FUNCTION__ << ": ";
            std::cout << " Left";
            std::cout << "\n";
        } else if (kbKey == KBKey::K_RIGHT) {
            movingState = MovingState::GOING_RIGHT;
            std::cout << __FUNCTION__ << ": ";
            std::cout << " Right";
            std::cout << "\n";
        } else if (kbKey == KBKey::K_DOWN) {
            std::cout << __FUNCTION__ << ": ";
            std::cout << " Down";
            std::cout << "\n";
        } else if (kbKey == KBKey::K_UP) {
            std::cout << __FUNCTION__ << ": ";
            std::cout << " Up";
            std::cout << "\n";
        }

    }
}
using namespace GAME;


int main(int argc, char **argv) {

    /* Init Game Technology */
    InitGameTech();
    GAME::SCENE::Init();

    /* Create Ship */
    auto ship = GAME::ShipFactory::CreateShip(ptech);
    GAME::ShipFactory::SetShipPosition(ship, WIN_WIDTH>>1, WIN_HEIGHT>>1);
 
    ECS::RenderingSystem::SubscribeEntity(ship);
    ECS::RenderingSystem::InitRenderingSystem(ptech);
    GAME::MainLoop();
    ECS::RenderingSystem::ShutdownRenderingSystem(ptech);

    
    /* Finish all the tech system (SDL for this case) */
    ptech->Finish();
    return 0;
}
