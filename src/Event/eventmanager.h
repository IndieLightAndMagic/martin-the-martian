#ifndef __EVENT_H__
#define __EVENT_H__

#include <map>
#include <vector>
#include <memory>
#include <utility>

#include "component.h"
#include "entity.h"
#include "Tech.h"

#include "SignalSlot/signalslot.h"
#include "Event/eventkeyboard.h"

namespace ECS {

	
	
	class EventManager : {

		using SignalVectorEventKey = std::vector<ECS::Signal<Tech2DEventKeyboard::KBEvent&, Tech2DEventKeyboard::KBKey&>>
		using PairKeyVector = std::pair<Tech2DEventKeyboard::KBEvent&, SignalVectorEventKey>;

	public:
		using ECSPEventManager = std::shared_ptr<EventManager>;
		static ECSPEventManager eventManager;
		void ProcessEvents()
		{

		}

		void RegisterKeyboardEvent(Tech2DEventKeyboard::KBEvent& rEvent, Tech2DEventKeyboard::KBKey& rKey, ECS::Signal<Tech2DEventKeyboard::KBEvent&, Tech2DEventKeyboard::KBKey &>& rSignal){

		}

	};
	EventManager::ECSPEventManager EventManager::eventManager = nullptr;
}

#endif /* __EVENT_H__ */