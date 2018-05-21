
#include <vector>
#include <functional>

#include <SDL2/SDL.h>
#include <ECS/System/rendering.h>
#include "Ship.h"

using namespace std;

void SDLWindowSize(int* w, int* h);


void RegisterKeyboardEvent(Uint32 event, Sint32 key, function<void(const Uint32&, const Sint32&)> slot);
void RegisterKeyboardEvents(Uint32 event, vector<Sint32>& keys, std::function<void(const Uint32&, const Sint32&)> slot);

void UpdateEvents();


namespace GAME{

    static unsigned int shipId;
    static bool bGameIsOn;

    void OnEscPressed(const Uint32&, const Sint32&);
    void OnArrowKeyPressed(const Uint32&, const Sint32&);


    void GameInitialization()
    {

        //My Keyboard slots
        auto arrowKeysGroup = std::vector<Sint32>{
                SDLK_RIGHT,
                SDLK_LEFT,
                SDLK_UP,
                SDLK_DOWN
        };
        RegisterKeyboardEvents(SDL_KEYDOWN, arrowKeysGroup, OnArrowKeyPressed);
        RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_ESCAPE, OnEscPressed);


        /* Create Ship */
        shipId = GAME::Ship::CreateShip();

        int width;
        int height;
        SDLWindowSize(&width, &height);
        GAME::Ship::SetShipPosition(shipId, width >> 1, height >> 1);


        ECS::RenderingSystem::InitRenderingSystem();
        ECS::RenderingSystem::SubscribeEntity(shipId);

    }

    void MainLoop()
    {
        bGameIsOn = true;
        while (bGameIsOn)
        {
            UpdateEvents();
            ECS::RenderingSystem::DrawSprites();
            ECS::RenderingSystem::UpdateRenderingSystem();
        }
    }
    void OnEscPressed(const Uint32& kbEvent, const Sint32& kbKey){
        std::cout << "GAME::OnEscPressed "  << __FUNCTION__ << std::endl;
        bGameIsOn = false;
    }

    void OnArrowKeyPressed(const Uint32& kbEvent, const Sint32& kbKey){


        if (kbKey ==  SDLK_LEFT){
            std::cout << __FUNCTION__ << ": ";
            std::cout << " Left";
            std::cout << "\n";
            GAME::Ship::SetShipPositionDelta(shipId,-64,0);
        } else if (kbKey == SDLK_RIGHT) {
            std::cout << __FUNCTION__ << ": ";
            std::cout << " Right";
            std::cout << "\n";
            GAME::Ship::SetShipPositionDelta(shipId,+64,0);
        } else if (kbKey == SDLK_DOWN) {
            std::cout << __FUNCTION__ << ": ";
            std::cout << " Down";
            std::cout << "\n";
        } else if (kbKey == SDLK_UP) {
            std::cout << __FUNCTION__ << ": ";
            std::cout << " Up";
            std::cout << "\n";
        }

    }

};
