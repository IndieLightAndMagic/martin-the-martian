#ifndef __TIMER_MNGR__
#define __TIMER_MNGR__
#include <timer.h>
#include <memory>
#include <vector>
#include <map>


namespace ECS {

	class TimedEventsManager_;
	using TimedEventsManager =  std::shared_ptr<ECS::TimedEventsManager_>;

	class TimedEventsManager_{
	public:
		static TimedEventsManager GetInstance();
		uint32_t RegisterTimer(TimedEvent &rTimedEvent);

	private:
		uint64_t counterReference;
        TimedEventsManager_();
        static TimedEventsManager_* pInstance;
	};
}

#endif  /*__TIMER_MNGR__*/