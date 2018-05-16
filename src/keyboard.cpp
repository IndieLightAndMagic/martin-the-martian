#include <functional>
#include <SDL2/SDL.h>
#include <SignalSlot/signalslot.h>
#include <vector>

using namespace std;
using namespace GTech;


using VectorLambda = vector<function<void()>>;
map<Uint64, VectorLambda> mDispatchRegisteredLambda;
vector<Uint64> vRegisteredKBEventsKeysMaskedPair;

void RegisterKeyboardEvent(Uint32 event, Sint32 key, std::function<void(const Uint32&, const Sint32&)> slot){


    auto sdlEvent64	= static_cast<Uint64>(event) << 32;
    auto sdlKey64 	= static_cast<Uint64>(key);
    auto mask64		= sdlEvent64 | sdlKey64;

    Signal<const Uint32&, const Sint32&> signal;
    signal.connect(slot);
    auto lf = [=](){
        signal.emit(event, key);
    };

    auto vLamb = mDispatchRegisteredLambda[mask64];
    if (vLamb.empty())
    {
        vRegisteredKBEventsKeysMaskedPair.push_back(mask64);
    }
    mDispatchRegisteredLambda[mask64].push_back(lf);
}

void RegisterKeyboardEvents(Uint32 event, vector<Sint32>& keys, std::function<void(const Uint32&, const Sint32&)> slot) {

    for (const auto key: keys)
    {
        RegisterKeyboardEvent(event, key, slot);
    }
}

void DispatchKeyboardEvents(SDL_Event& rEvent){

    const auto ev   = rEvent.type;
    if (ev  != SDL_KEYDOWN && ev  != SDL_KEYUP) return;
    const auto key  = rEvent.key.keysym.sym;
    auto mask64     = static_cast<Uint64>(ev );
    mask64        <<= 32;
    mask64         |= key;
    auto beginmask  = vRegisteredKBEventsKeysMaskedPair.begin();
    auto endmask    = vRegisteredKBEventsKeysMaskedPair.end();
    auto it         = std::find(beginmask, endmask, mask64);
    if (it == endmask) return;

    auto vLambda    = mDispatchRegisteredLambda[mask64];
    for (auto& lambda : vLambda)
    {
        lambda();
    }
}
