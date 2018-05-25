#include <timermanager.h>



using namespace std;
using namespace ECS;

TimedEventsManager_* TimedEventsManager_::pInstance = nullptr;
TimedEventsManager_::TimedEventsManager_(){

}

TimedEventsManager TimedEventsManager_::GetInstance(){

	if (!pInstance) {
		auto ptr = new TimedEventsManager_;
		pInstance = ptr;
	}
	return std::shared_ptr<TimedEventsManager_>(pInstance);

}

uint32_t TimedEventsManager_::RegisterTimer(TimedEvent &rTimedEvent) {

	return 0;

}