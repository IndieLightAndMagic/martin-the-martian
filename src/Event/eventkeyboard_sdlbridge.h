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

        constexpr Uint32 SDLKBEvent(GTech::KBEvent& rKBEvent);
        constexpr GTech::KBEvent KBEvent_SDL(Uint32& rSDLKBEvent);

    }
    namespace KeyboardKey{
        constexpr GTech::KBKey KBKey_SDL(Sint32& rSDLKBKey);
        constexpr Sint32 SDLKBKey(GTech::KBKey & rKBKey);
    }
    /*!
    \brief Register a generic KBEvent, for a generic keybutton, with a generic Signal.
    */
    void RegisterKeyboardEvent_SDL(KBEvent& rEvent, KBKey& rKeyButton, Signal<KBEvent&, KBKey&>& rSignal);
    void DispatchKeyboardEvent_SDL(Uint32& rKBEvent, Sint32& rKBKey);



}


#endif /* __EVENTKEYBOARD_SDL_BRIDGE_H__ */
