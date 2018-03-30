#ifndef __EVENTKEYBOARD_H__
#define __EVENTKEYBOARD_H__

#include <queue>

#include "component.h"
#include "entity.h"
#include "Tech.h"

namespace GTech {

	struct Tech2DEventKeyboard : public Tech2DEvent{
	public:
		enum struct KBEvent{KEY_PRESSED, KEY_RELEASED};
		enum struct KBKey{
			K_UP,
			K_LEFT,
			K_DOWN,
			K_RIGHT,

			K_ESC,
			K_SPACE,

			K_W,
			K_A,
			K_S,
			K_D
		};
		Tech2DEventKeyboard(Tech2DEventKeyboard::KBEvent event, Tech2DEventKeyboard::KBKey key):m_event(event), m_key(key){

		}
		KBEvent m_event;
		KBKey m_key;
	};

}

#endif /* __EVENTKEYBOARD_H__ */