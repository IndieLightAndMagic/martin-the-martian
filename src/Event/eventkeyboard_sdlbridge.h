#ifndef __EVENTKEYBOARD_SDL_BRIDGE_H__
#define __EVENTKEYBOARD_SDL_BRIDGE_H__


#include "Tech.h"
#include "Tech_SDLBridge.h"

#include "SignalSlot/signalslot.h"
#include "Event/eventkeyboard.h"

using namespace std;

namespace GTech{

    namespace KeyboardEventDispatch{

    }
    namespace KeyboardEvent{

        constexpr Uint32 SDLKBEvent(const GTech::KBEvent& rKBEvent);
        constexpr GTech::KBEvent KBEvent_SDL(const Uint32& rSDLKBEvent);

    }
    namespace KeyboardKey{
        constexpr GTech::KBKey KBKey_SDL(const Sint32& rSDLKBKey);
        constexpr Sint32 SDLKBKey(const GTech::KBKey & rKBKey);
    }
    /*!
    \brief Register a generic KBEvent, for a generic keybutton, with a generic Signal.
    */
    


}


#endif /* __EVENTKEYBOARD_SDL_BRIDGE_H__ */
