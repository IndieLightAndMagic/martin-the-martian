#include <timermanager.h>
#include <timer.h>


using namespace std;
using namespace ECS;

static vector<TimedEvent> vTimedEvents;


const TimedEventsSystem& TimedEventsSystem::GetInstance(){

    static TimedEventsSystem em;

    return em;

}

uint32_t TimedEventsSystem::RegisterTimer(TimedEvent &rTimedEvent) {

	rTimedEvent->SetCounterReference(&counterReference);
	return 0;

}
