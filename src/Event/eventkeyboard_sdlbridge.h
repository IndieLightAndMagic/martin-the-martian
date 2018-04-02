#ifndef __EVENTKEYBOARD_SDL_BRIDGE_H__
#define __EVENTKEYBOARD_SDL_BRIDGE_H__

#include <map>

#include "Tech.h"
#include "Tech_SDLBridge.h"

#include "SignalSlot/signalslot.h"
#include "Event/eventkeyboard.h"

using namespace std;

namespace GTech{

    namespace KeyboardEventDispatch{
        map<Uint32, map<Sint32, vector<Signal<KBEvent&, KBKey&>>>> keyboardEventDispatch;
    }
    namespace KeyboardEvent{
        const map<KBEvent, Uint32> SDLGTechMap = {{KBEvent::KEY_PRESSED, SDL_KEYDOWN}, {KBEvent::KEY_RELEASED, SDL_KEYUP}};
        const map<KBEvent, Uint32> GTechSDLMap = {{SDL_KEYDOWN, KBEvent::KEY_PRESSED}, {SDL_KEYUP, KBEvent::KEY_RELEASED}};
    }
    namespace KeyboardKey{
        const map<KBKey, Sint32> GTechSDLMap{
                //******************************
                {SDLK_a,        KBKey::K_A},
                {SDLK_d,        KBKey::K_D},
                {SDLK_w,        KBKey::K_W},
                {SDLK_s,        KBKey::K_S},
                //******************************
                {SDLK_UP,       KBKey::K_UP},
                {SDLK_DOWN,     KBKey::K_DOWN},
                {SDLK_LEFT,     KBKey::K_LEFT},
                {SDLK_RIGHT,    KBKey::K_RIGHT},
                //******************************
                {SDLK_ESCAPE,   KBKey::K_ESC},
                {SDLK_SPACE,    KBKey::K_SPACE}
                //******************************
            };
        const map<Sint32, KBKey> SDLGTechMap{
                //******************************
                {KBKey::K_A,        SDLK_a},
                {KBKey::K_D,        SDLK_d},
                {KBKey::K_W,        SDLK_w},
                {KBKey::K_S,        SDLK_s},
                //******************************
                {KBKey::K_UP,       SDLK_UP},
                {KBKey::K_DOWN,     SDLK_DOWN},
                {KBKey::K_LEFT,     SDLK_LEFT},
                {KBKey::K_RIGHT,    SDLK_RIGHT},
                //******************************
                {KBKey::K_ESC,      SDLK_ESCAPE},
                {KBKey::K_SPACE,    SDLK_SPACE}
                //******************************
        };
    };
    using namespace ECS;
    void RegisterKeyboardEvent(KBEvent& rEvent, KBKey& rKeyButton, Signal<KBEvent&, KBKey&>& rSignal){

        SDL_assert(KeyboardKey::GTechSDLMap.find(rKeyButton) != KeyboardKey::GTechSDLMap.end());
        SDL_assert(KeyboardEvent::GTechSDLMap.find(rEvent) != KeyboardEvent::GTechSDLMap.end());

        using namespace KeyboardEventDispatch;
        auto keySignalMap = keyboardEventDispatch[KeyboardEvent::GTechSDLMap[rEvent]];
        auto eventKeySignalsVector = keySignalMap[KeyboardKey::GTechSDLMap[rKeyButton]];
        eventKeySignalsVector.push_back(rSignal);
    }
    void DispatchKeyboardEvent(Uint32 sdlevent, Sint32 sdlkeycode){

        using namespace KeyboardEventDispatch;
        for (auto& signal:keyboardEventDispatch[sdlevent][sdlkeycode]){
            signal.emit(KeyboardEvent::SDLGTechMap[sdlevent], KeyboardKey::SDLGTechMap[sdlkeycode]);
        }

    }

}


#endif /* __EVENTKEYBOARD_SDL_BRIDGE_H__ */
