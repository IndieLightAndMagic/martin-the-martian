#include "eventkeyboard_sdlbridge.cpp"

using namespace ECS;
void RegisterKeyboardEvent(KBEvent& rEvent, KBKey& rKeyButton, Signal<KBEvent&, KBKey&>& rSignal){

    //SDL_assert(KeyboardKey::GTechSDLMap.find(rKeyButton) != KeyboardKey::GTechSDLMap.end());
    //SDL_assert(KeyboardEvent::GTechSDLMap.find(rEvent) != KeyboardEvent::GTechSDLMap.end());

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