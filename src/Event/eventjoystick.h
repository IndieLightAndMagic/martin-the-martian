#ifndef __EVENTJOYSTICK_H__
#define __EVENTJOYSTICK_H__

#include "Event/event.h"

namespace GTech{

    enum struct JoyHatEvent{HAT_CHANGED};
    enum struct JoyHatPosition : u_int8_t {

        DEAD_ZONE       = 0x0,
        UP_DIRECTION    = 0x1,
        LT_DIRECTION    = 0x2,
        UPLT_DIRECTION  = 0x3,
        DW_DIRECTION    = 0x4,
        DWLT_DIRECTION  = 0x6,
        RT_DIRECTION    = 0x8,
        DWRT_DIRECTION  = 0xE

    };

    enum struct JoyAxisEvent{
        AXIS_0_MOVED,
        AXIS_1_MOVED,
        AXIS_2_MOVED,
        AXIS_3_MOVED
    };
    struct JoystickAxis {

        static float scale;
        static float min;
        static float max;
        float value;

    };
    float JoystickAxis::scale = 1.0f;
    float JoystickAxis::min = 0.0f;
    float JoystickAxis::max = 1.0f;

}

#endif