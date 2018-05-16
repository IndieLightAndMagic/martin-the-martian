
#include "ECS/Component/componentmanager.h"
#include "Tech/SDL/Tech_SDLBridge.h"
#include "Ship.h"


void SDLInitialization();
void SDLFinisih();

void GameInitialization(std::function<void(const Uint32&, const Sint32&)> slotKeyEsc, std::function<void(const Uint32&, const Sint32&)> slotKeyDown);

void OnEscPressed(const Uint32&, const Sint32&);
void OnArrowKeyPressed(const Uint32&, const Sint32&);

bool bGameIsOn;

void OnEscPressed(const Uint32& kbEvent, const Sint32& kbKey){
    std::cout << "GAME::OnEscPressed "  << __FUNCTION__ << std::endl;
    bGameIsOn = false;
}
void OnArrowKeyPressed(const Uint32& kbEvent, const Sint32& kbKey){


    if (kbKey ==  SDLK_LEFT){
        std::cout << __FUNCTION__ << ": ";
        std::cout << " Left";
        std::cout << "\n";
    } else if (kbKey == SDLK_RIGHT) {
        std::cout << __FUNCTION__ << ": ";
        std::cout << " Right";
        std::cout << "\n";
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

void MainLoop()
{

    while (bGameIsOn)
    {

    }
}
int main(int argc, char **argv) {

    /* Init Game Technology */
    SDLInitialization();
    GameInitialization(OnEscPressed, OnArrowKeyPressed);




    
    /* Finish all the tech system (SDL for this case) */
    SDL_Quit();
    return 0;
}
