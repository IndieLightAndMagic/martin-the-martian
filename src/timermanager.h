#ifndef __TIMER_MNGR__
#define __TIMER_MNGR__
#include <timer.h>
#include <memory>
#include <vector>
#include <map>

namespace ECS {

	class TimedEventsSystem{
	public:
		static const TimedEventsSystem& GetInstance();
		uint32_t RegisterTimer(TimedEvent &rTimedEvent);

	private:
		uint64_t counterReference;
        TimedEventsSystem() 	= default;
        ~TimedEventsSystem()	= default;

        //Invalidate 4 out of the remaining "rule of 5" ctors.   
		TimedEventsSystem(const TimedEventsSystem&)	= delete;
		TimedEventsSystem(TimedEventsSystem&&)		= delete;
		const TimedEventsSystem& operator=(const TimedEventsSystem&)    = delete;
		const TimedEventsSystem operator=(TimedEventsSystem&&)          = delete;
	};

}

#endif  /*__TIMER_MNGR__*/