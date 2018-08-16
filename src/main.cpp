#include <sdlwrapper.h>
// #include "sdlwrapper.h"
#include "timer.h"

using namespace GTech;

namespace GAME{
    void GameInitialization();
    void MainLoop();
}



int main(int argc, char **argv) {

    /* Init Game Technology */
    SDLInitialization();



    /* Initialization of the game */
    GAME::GameInitialization();
    GAME::MainLoop();



    /* Finish all the tech system (SDL for this case) */
    SDLQuit();
    return 0;
}
