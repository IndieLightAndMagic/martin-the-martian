#include "eventkeyboard_sdlbridge.h"

#include <map>
#include <vector>
#include <functional>

using namespace std;
namespace GTech {
	namespace KeyboardEventDispatcher{

		vector<function<void()>> vSDLKBEventDispatch;
		using vectorSignalsKBEventKey = vector<Signal<GTech::KBEvent&, GTech::KBKey&>&>;
		using vectorLambda = vector<function<void()>>;
		map<Uint64, vectorLambda> mDispatchRegisteredLambda;
		vector<Uint64> vRegisteredKBEvents;


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
	template <typename... Args>
	void RegisterKeyboardEvent_SDL(const KBEvent& rKBEvent, const KBKey& rKBKey, std::function<void(Args...)> slot){

		auto sdlEvent64	= static_cast<Uint64>(KeyboardEvent::SDLKBEvent(rKBEvent)) << 32;
		auto sdlKey64 	= static_cast<Uint64>(KeyboardKey::SDLKBKey(rKBKey));
		auto mask64		= sdlEvent64 | sdlKey64;

		Signal<const KBEvent&, const KBKey&>signal;
		signal.connect(slot);

		auto lf = [=](){
			signal.emit(rKBEvent,rKBKey);
		};

	}

	void DispatchKeyboardEvent_SDL(Uint32& rKBEvent,  Sint32& rKBKey){

		auto mask64 = (static_cast<Uint64>(rKBEvent) << 32) | static_cast<Uint64>(rKBEvent);
		for (const auto& registeredKBEvent: KeyboardEventDispatcher::vRegisteredKBEvents){

			if (mask64 != registeredKBEvent) continue;
			auto vLambdas	= KeyboardEventDispatcher::mDispatchRegisteredLambda[mask64];
			for (const auto& lambda: vLambdas)
				lambda();

		}
	}	
}
