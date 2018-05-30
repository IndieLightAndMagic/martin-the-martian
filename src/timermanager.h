#ifndef __TIMER_MNGR__
#define __TIMER_MNGR__
#include <timer.h>
#include <memory>
#include <vector>
#include <map>


namespace ECS {

	class TimedEventsManager;

	class TimedEventsManager{
	public:
		static const TimedEventsManager& GetInstance();
		uint32_t RegisterTimer(TimedEvent &rTimedEvent);
		
	private:
		uint64_t counterReference;
        TimedEventsManager() = default;

	};
}

#endif  /*__TIMER_MNGR__*/