#include "eventkeyboard_sdlbridge.h"

namespace GTech {

	namespace KeyboardEvent{
        constexpr Uint32 SDLKBEvent(GTech::KBEvent& rKBEvent){
            switch(rKBEvent){
                case GTech::KBEvent::KEY_PRESSED:
                    return SDL_KEYDOWN;
                    break;
                case GTech::KBEvent::KEY_RELEASED:
                    return SDL_KEYUP;
                    break;
                default:
                    return 0;
            }
        };
        constexpr GTech::KBEvent KBEvent(Uint32& rSDLKBEvent){
            switch(rSDLKBEvent){
                case SDL_KEYDOWN:
                    return GTech::KBEvent::KEY_PRESSED;
                    break;
                case SDL_KEYUP:
                    return GTech::KBEvent::KEY_RELEASED;
                    break;
                default:
                    return GTech::KBEvent::KEY_UNKNOWNEVENT;
            }
        };
    }
    namespace KeyboardKey{
    	constexpr GTech::KBKey KBKey(Sint32& rSDLKBKey){
    		switch(rSDLKBKey){
    			case SDLK_a:        
                	return GTech::KBKey::K_A;
                	break;
                case SDLK_d:        
                	return GTech::KBKey::K_D;
                	break;
                case SDLK_w:        
                	return GTech::KBKey::K_W;
                	break;
                case SDLK_s:        
                	return GTech::KBKey::K_S;
                	break;
                
                case SDLK_UP:       
                	return GTech::KBKey::K_UP;
                	break;
                case SDLK_DOWN:     
                	return GTech::KBKey::K_DOWN;
                	break;
                case SDLK_LEFT:     
                	return GTech::KBKey::K_LEFT;
                	break;
                case SDLK_RIGHT:    
                	return GTech::KBKey::K_RIGHT;
                	break;
                
                case SDLK_ESCAPE:   
                	return GTech::KBKey::K_ESC;
                	break;
                case SDLK_SPACE:    
                	return GTech::KBKey::K_SPACE;
                	break;
                case SDLK_UNKNOWN:
                default:
                	return GTech::KBKey::K_UNKNOWN;
                	break;
    		}
		};
		constexpr Sint32 SDLKBKey(GTech::KBKey & rKBKey){
    		switch(rKBKey){
    			case GTech::KBKey::K_A :        
                	return SDLK_a;
                	break;
                case GTech::KBKey::K_D :        
                	return SDLK_d;
                	break;
                case GTech::KBKey::K_W :        
                	return SDLK_w;
                	break;
                case GTech::KBKey::K_S :        
                	return SDLK_s;
                	break;
                case GTech::KBKey::K_UP :       
                	return SDLK_UP;
                	break;
                case GTech::KBKey::K_DOWN :     
                	return SDLK_DOWN;
                	break;
                case GTech::KBKey::K_LEFT :     
                	return SDLK_LEFT;
                	break;
                case GTech::KBKey::K_RIGHT :    
                	return SDLK_RIGHT;
                	break;
                case GTech::KBKey::K_ESC :   
                	return SDLK_ESCAPE;
                	break;
                case GTech::KBKey::K_SPACE :    
                	return SDLK_SPACE;
                	break;
                case GTech::KBKey::K_UNKNOWN :
                	return SDLK_UNKNOWN;
                	break;
                default:
					return SDLK_UNKNOWN;
                	break;
    		}
		};
        
    }

	void RegisterKeyboardEvent(KBEvent& rEvent, KBKey& rKeyButton, Signal<KBEvent&, KBKey&>& rSignal){

	    //SDL_assert(KeyboardKey::GTechSDLMap.find(rKeyButton) != KeyboardKey::GTechSDLMap.end());
	    //SDL_assert(KeyboardEvent::GTechSDLMap.find(rEvent) != KeyboardEvent::GTechSDLMap.end());
		auto dispatcher = [=](){
			rSignal.emit(rEvent, rKeyButton);
		};
		std::vector<decltype(dispatcher)>dispatcherVector;

		


	}

	void DispatchKeyboardEvent(Uint32 sdlevent, Sint32 sdlkeycode){

		using namespace KeyboardEventDispatch;
	    for (auto& signal:keyboardEventDispatch[sdlevent][sdlkeycode]){
	        signal.emit(KeyboardEvent::SDLGTechMap[sdlevent], KeyboardKey::SDLGTechMap[sdlkeycode]);
	    }
	}	
}
