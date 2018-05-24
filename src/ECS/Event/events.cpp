

#include <SDL2/SDL_events.h>
#include "events.h"

void DispatchKeyboardEvents(SDL_Event& rEvent);
namespace ECS{

    void UpdateEvents() {

        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            //Dispatch registered Keyboard Events.
            DispatchKeyboardEvents(e);
        }

    }
}
