#ifndef __EVENTKEYBOARD_H__
#define __EVENTKEYBOARD_H__

#include <queue>

namespace GTech{

	enum KBEvent{KEY_PRESSED, KEY_RELEASED, KEY_UNKNOWNEVENT};
	enum KBKey{
	
		K_UP,
		K_LEFT,
		K_DOWN,

		K_RIGHT,
		K_ESC,
		K_SPACE,

		K_W,
		K_A,
		K_S,
		K_D,

		K_UNKNOWN

	};



}

#endif /* __EVENTKEYBOARD_H__ */