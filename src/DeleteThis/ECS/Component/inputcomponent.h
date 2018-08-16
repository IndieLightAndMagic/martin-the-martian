#ifndef __INPUT_COMPONENT_H__
#define __INPUT_COMPONENT_H__

#include <Event/eventmanager.h>
#include <Event/eventkeyboard.h>
#include <Event/eventgamepad.h>
#include <Event/eventjoystick.h>
// #include <SignalSlot/signalslot.h> // dunno
#include "SignalSlot/signalslot.h"
#include <ECS/Component/component.h>
#include <Event/eventgamepad.h>

namespace ECS {

    class InputComponent : public Component_ {

        GTech::Signal<GTech::KBEvent&, GTech::KBKey&> keyboardSignal;
        GTech::Signal<GTech::GPEvent&, GTech::GPKey&> gamepadSignal;
        GTech::Signal<GTech::JoyAxisEvent&, GTech::JoystickAxis&> joystickSignal;
        enum class JOYHATPOSITION : u_int8_t {

            DEAD_ZONE       = 0x0,
            UP_DIRECTION    = 0x1,
            LT_DIRECTION    = 0x2,
            UPLT_DIRECTION  = 0x3,
            DW_DIRECTION    = 0x4,
            DWLT_DIRECTION  = 0x6,
            RT_DIRECTION    = 0x8,
            DWRT_DIRECTION  = 0xE
        };
        std::vector<JOYHATPOSITION> hatpositions;
        unsigned int m_joysticksPerGamepad;
    public:
        InputComponent(unsigned int nJoysticksPerGamepad = 2):m_joysticksPerGamepad(nJoysticksPerGamepad) {

            //Generate Connections
            keyboardSignal.connect_member(this, &InputComponent::OnKeyboardEvent);


            if (!nJoysticksPerGamepad) return;
            while (nJoysticksPerGamepad--) hatpositions.push_back(JOYHATPOSITION::DEAD_ZONE);
            gamepadSignal.connect_member(this, &InputComponent::OnGamepadEvent);
            joystickSignal.connect_member(this, &InputComponent::OnJoyAxisEvent);

            //Register the connections with the underlying technology

        }
        void OnKeyboardEvent(GTech::KBEvent&, GTech::KBKey& key){

        }
        void OnGamepadEvent(GTech::GPEvent&, GTech::GPKey& button){

        }
        void OnJoyAxisEvent(GTech::JoyAxisEvent& axis, GTech::JoystickAxis& axisValue)
        {

        }
    };

}

#endif /* __INPUT_COMPONENT_H__ */
