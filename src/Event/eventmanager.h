#ifndef __EVENT_H__
#define __EVENT_H__

#include <map>
#include <vector>
#include <memory>
#include <utility>

#include <ECS/Component/component.h>
#include "entity.h"
#include "Tech/G/Tech.h"

#include "SignalSlot/signalslot.h"
#include "Event/eventkeyboard.h"

namespace ECS
{
    using namespace std;
    class EventManager {


        using PEventManager = std::shared_ptr<EventManager>;
        static PEventManager eventManager;

        EventManager(){}
    public:

        static PEventManager& GetEventManager() {
            if (eventManager == nullptr)
            {
                eventManager = std::make_shared(new (decltype(eventManager)));
            }
            return eventManager;
        }
        template <class T, class Ev, class Ob>
        void RegisterEventKBEvent(KBEvent revent, KBKey rkey, T* pinst, void(T::*slot)(Ev, Ob)) {



        };



	};
	EventManager::PEventManager EventManager::eventManager = nullptr;
}

#endif /* __EVENT_H__ */