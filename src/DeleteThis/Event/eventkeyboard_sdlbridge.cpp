#include "Tech/SDL/Tech_SDLBridge.h"

#include <map>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;
namespace GTech {
	namespace KeyboardEventDispatcher{

		vector<function<void()>> vSDLKBEventDispatch;
		using vectorSignalsKBEventKey = vector<Signal<GTech::KBEvent&, GTech::KBKey&>&>;
		using vectorLambda = vector<function<void()>>;
		map<Uint64, vectorLambda> mDispatchRegisteredLambda;
		vector<Uint64> vRegisteredKBEventsKeysMaskedPair;

	}
	namespace KeyboardEvent{
        constexpr Uint32 SDLKBEvent(const GTech::KBEvent& rKBEvent){
            switch(rKBEvent){
                case GTech::KBEvent::KEY_PRESSED:
                    return SDL_KEYDOWN;

                case GTech::KBEvent::KEY_RELEASED:
                    return SDL_KEYUP;

                default:
                    return 0;
            }
        };
        constexpr GTech::KBEvent KBEvent_SDL(const Uint32& rSDLKBEvent){
            switch(rSDLKBEvent){
                case SDL_KEYDOWN:
                    return GTech::KBEvent::KEY_PRESSED;

                case SDL_KEYUP:
                    return GTech::KBEvent::KEY_RELEASED;

                default:
                    return GTech::KBEvent::KEY_UNKNOWNEVENT;
            }
        };

    }
    namespace KeyboardKey{
    	constexpr GTech::KBKey KBKey_SDL(const Sint32& rSDLKBKey){
    		switch(rSDLKBKey){
    			case SDLK_a:        
                	return GTech::KBKey::K_A;

                case SDLK_d:        
                	return GTech::KBKey::K_D;

                case SDLK_w:        
                	return GTech::KBKey::K_W;

                case SDLK_s:        
                	return GTech::KBKey::K_S;

                
                case SDLK_UP:       
                	return GTech::KBKey::K_UP;

                case SDLK_DOWN:     
                	return GTech::KBKey::K_DOWN;

                case SDLK_LEFT:     
                	return GTech::KBKey::K_LEFT;

                case SDLK_RIGHT:    
                	return GTech::KBKey::K_RIGHT;

                case SDLK_ESCAPE:   
                	return GTech::KBKey::K_ESC;

                case SDLK_SPACE:    
                	return GTech::KBKey::K_SPACE;

                case SDLK_UNKNOWN:
                default:
                	return GTech::KBKey::K_UNKNOWN;

    		}
		};

		constexpr Sint32 SDLKBKey(const GTech::KBKey & rKBKey){
    		switch(rKBKey){

    			case GTech::KBKey::K_A :        
                	return SDLK_a;

                case GTech::KBKey::K_D :        
                	return SDLK_d;

                case GTech::KBKey::K_W :        
                	return SDLK_w;

                case GTech::KBKey::K_S :        
                	return SDLK_s;

                case GTech::KBKey::K_UP :       
                	return SDLK_UP;

                case GTech::KBKey::K_DOWN :     
                	return SDLK_DOWN;

                case GTech::KBKey::K_LEFT :     
                	return SDLK_LEFT;

                case GTech::KBKey::K_RIGHT :    
                	return SDLK_RIGHT;

                case GTech::KBKey::K_ESC :   
                	return SDLK_ESCAPE;

                case GTech::KBKey::K_SPACE :    
                	return SDLK_SPACE;

                case GTech::KBKey::K_UNKNOWN :
                	return SDLK_UNKNOWN;

                default:
					return SDLK_UNKNOWN;
    		}
		};
        
    }
    void Tech_SDLBridge::RegisterKeyboardEvent(const KBEvent& rKBEvent, std::vector<const KBKey> vKBKeys, std::function<void(const KBEvent&, const KBKey&)> slot)
    {
        for (const auto key : vKBKeys)
        {
            RegisterKeyboardEvent(rKBEvent,key,slot);
        }
    }
	void Tech_SDLBridge::RegisterKeyboardEvent(const KBEvent& rKBEvent, const KBKey& rKBKey, std::function<void(const KBEvent&, const KBKey&)> slot){


        auto sdlKBEvent = KeyboardEvent::SDLKBEvent(rKBEvent);
        auto sdlEvent64	= static_cast<Uint64>(sdlKBEvent) << 32;
		auto sdlKBKey   = KeyboardKey::SDLKBKey(rKBKey);
        auto sdlKey64 	= static_cast<Uint64>(sdlKBKey);
		auto mask64		= sdlEvent64 | sdlKey64;

		Signal<const KBEvent&, const KBKey&> signal;
		signal.connect(slot);
		auto lf = [=](){
			signal.emit(rKBEvent, rKBKey);
		};
		auto vLamb = KeyboardEventDispatcher::mDispatchRegisteredLambda[mask64];
		if (vLamb.empty())
		{
			KeyboardEventDispatcher::vRegisteredKBEventsKeysMaskedPair.push_back(mask64);
		}
        KeyboardEventDispatcher::mDispatchRegisteredLambda[mask64].push_back(lf);
	}

	void Tech_SDLBridge::DispatchKeyboardEvents(SDL_Event& rEvent){

        const auto ev   = rEvent.type;
        if (ev  != SDL_KEYDOWN && ev  != SDL_KEYUP) return;
        const auto key  = rEvent.key.keysym.sym;
        auto mask64     = static_cast<Uint64>(ev );
        mask64        <<= 32;
        mask64         |= key;
        auto beginmask  = KeyboardEventDispatcher::vRegisteredKBEventsKeysMaskedPair.begin();
        auto endmask    = KeyboardEventDispatcher::vRegisteredKBEventsKeysMaskedPair.end();
        auto it         = std::find(beginmask, endmask, mask64);
        if (it == endmask) return;

        auto vLambda    = KeyboardEventDispatcher::mDispatchRegisteredLambda[mask64];
        for (auto& lambda : vLambda)
        {
            lambda();
        }
    }

}
