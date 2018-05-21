
void SDLInitialization();
void SDLQuit();

namespace GAME{
    void GameInitialization();
    void MainLoop();
}



int main(int argc, char **argv) {

    /* Init Game Technology */
    SDLInitialization();

    GAME::GameInitialization();
    GAME::MainLoop();

    
    /* Finish all the tech system (SDL for this case) */
    SDLQuit();
    return 0;
}
