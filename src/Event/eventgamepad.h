#ifndef __EVENTGAMEPAD_H__
#define __EVENTGAMEPAD_H__

#include "Event/event.h"

namespace GTech2D{

	
    struct Tech2DEventGamepad : public Tech2DEvent{
    public:
        enum struct KPEvent{KEY_PRESSED, KEY_RELEASED};
        enum struct KPKey{
            B_SELECT    = 0,

            B_JOYLFTCLK = 1,
            B_JOYRGTCLK = 2,

            B_START     = 3,

            B_UP        = 4,
            B_RGHT      = 5,
            B_DOWN      = 6,
            B_LEFT      = 7,

            B_TRIGLEFT  = 8,
            B_TRIGRGHT  = 9,

            B_SHOULDLFT = 10,
            B_SHOULDRGT = 11,

            B_TRIANGLE  = 12,
            B_CIRCLE    = 13,
            B_CROSS     = 14,
            B_SQUARE    = 15,

            B_PS        = 16
        };

        KPEvent m_event;
        KPKey m_key;
    };

}

#endif