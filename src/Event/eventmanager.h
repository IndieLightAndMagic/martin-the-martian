#ifndef __EVENT_H__
#define __EVENT_H__

#include <map>
#include <vector>
#include <memory>
#include <utility>

#include <ECS/Component/component.h>
#include "entity.h"
#include "Tech.h"

#include "SignalSlot/signalslot.h"
#include "Event/eventkeyboard.h"

namespace ECS
{

    class EventManager;

	class EventManager {

		using SignalVectorEventKey = std::vector<ECS::Signal<GTech::KBEvent&, GTech::KBKey&>>

        using PEventManager = std::shared_ptr<EventManager>;
        using PEventManagerWeak = std::weak_ptr<EventManager>;

        static PEventManager eventManager;

        EventManager(){}
    public:

        static PEventManager& GetEventManager()
        {
            if (eventManager == nullptr)
            {
                eventManager = std::make_shared(new (decltype(eventManager)));
            }
            return eventManager;
        }
        template <typename TEvent, typename TKeyButton>
		void RegisterKeyboardKeySignal(
				TEvent& rEvent,
				TKeyButton& rKeyButton,
				ECS::Signal<TEvent&, TKeyButton&>& rSignal){



		}
        virtual void ProcessEvents() = 0;
	};
	EventManager::PEventManager EventManager::eventManager = nullptr;
}

#endif /* __EVENT_H__ */