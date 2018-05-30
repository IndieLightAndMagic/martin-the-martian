#include <timermanager.h>
#include <timer.h>


using namespace std;
using namespace ECS;

static vector<TimedEvent> vTimedEvents;


const TimedEventsManager& TimedEventsManager::GetInstance(){

    static TimedEventsManager em;

    return em;

}

uint32_t TimedEventsManager::RegisterTimer(TimedEvent &rTimedEvent) {

	rTimedEvent->SetCounterReference(&counterReference);
	return 0;

}