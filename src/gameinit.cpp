
#include <vector>
#include <functional>

#include <SDL2/SDL.h>
#include <ECS/System/rendering.h>
#include "Ship.h"

using namespace std;

void SDLWindowSize(int* w, int* h);


void RegisterKeyboardEvent(Uint32 event, Sint32 key, function<void(const Uint32&, const Sint32&)> slot);
void RegisterKeyboardEvents(Uint32 event, vector<Sint32>& keys, std::function<void(const Uint32&, const Sint32&)> slot);

void GameInitialization(std::function<void(const Uint32&, const Sint32&)> slotKeyEsc, std::function<void(const Uint32&, const Sint32&)> slotKeyDown)
{

    //My Keyboard slots
    auto arrowKeysGroup = std::vector<Sint32>{
            SDLK_RIGHT,
            SDLK_LEFT,
            SDLK_UP,
            SDLK_DOWN
    };
    RegisterKeyboardEvents(SDL_KEYDOWN, arrowKeysGroup, slotKeyDown);
    RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_ESCAPE, slotKeyEsc);


    /* Create Ship */
    auto ship = GAME::Ship::CreateShip();

    int width;
    int height;
    SDLWindowSize(&width, &height);
    GAME::Ship::SetShipPosition(ship, width >> 1, height >> 1);


    ECS::RenderingSystem::InitRenderingSystem();
    ECS::RenderingSystem::SubscribeEntity(ship);



}
