
#include <vector>
#include <functional>

#include <SDL2/SDL.h>
#include <ECS/System/kinematics.h>
#include <ECS/System/rendering.h>
#include "Ship.h"
#include "ECS/Event/events.h"
#include "Bolt.h"

using namespace std;

void SDLWindowSize(int* w, int* h);


void RegisterKeyboardEvent(Uint32 event, Sint32 key, function<void(const Uint32&, const Sint32&)> slot);
void RegisterKeyboardEvents(Uint32 event, vector<Sint32>& keys, std::function<void(const Uint32&, const Sint32&)> slot);



namespace GAME{

    static unsigned int shipId;
    static bool bGameIsOn;

    void OnEscPressed(const Uint32&, const Sint32&);
    void OnArrowKeyPressed(const Uint32&, const Sint32&);
    void OnFirePressed(const Uint32&, const Sint32&);


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
        RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_SPACE, OnFirePressed);


        /* Create Ship */
        auto shipTexturePath = std::string(RES_DIR)+"hero.png";
        shipId = GAME::Ship::CreateShip(shipTexturePath);

        int width;
        int height;
        SDLWindowSize(&width, &height);
        GAME::Ship::SetShipPosition(shipId, width >> 1, height >> 1);

        //Init Systems
        ECS::RenderingSystem::InitRenderingSystem();
        ECS::KinematicsSystem::InitKinematicsSystem();

        //Subscribe Entity into Systems
        ECS::RenderingSystem::SubscribeEntity(shipId);

    }

    void MainLoop()
    {
        bGameIsOn = true;
        while (bGameIsOn)
        {
            ECS::UpdateEvents();
            ECS::RenderingSystem::DrawSprites();
            ECS::RenderingSystem::UpdateRenderingSystem();
            ECS::KinematicsSystem::UpdateKinematicsSystem();
        }
    }

    void OnFirePressed(const uint32_t& kbEvent, const int32_t& kbKey){

        auto resPath = std::string(RES_DIR)+"orangebolt.png";
        auto boltId = GAME::Bolt::CreateBolt(resPath);
        ECS::KinematicsSystem::SubscribeEntity(boltId);

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
