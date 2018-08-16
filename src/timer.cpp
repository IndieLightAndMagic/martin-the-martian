#include <SDL2/SDL.h>
#include <timer.h>
#include <map>
#include <functional>
#include <vector>
#include <memory>

#include <SignalSlot/signalslot.h>

using namespace std;
using namespace GTech;

static unsigned int timedEventId = 0;
using namespace ECS;

static Uint64 timerCounterReference = 0;
const uint64_t TimedEvent_::m_tickCounterFrequencyHz    = SDL_GetPerformanceFrequency();
const double_t TimedEvent_::m_tickCounterPeriodMs 	    = 1000 / static_cast<double>(m_tickCounterFrequencyHz);

TimedEvent_::TimedEvent_(Uint32 delayMs) {

    m_ptrTNow = &timerCounterReference;
	Set(delayMs);


}

void TimedEvent_::Set(Uint32 delayMs){

    static auto f_Hz_3  = m_tickCounterFrequencyHz / 1000.0;
    m_intervalms 	    = delayMs;
    auto intervalTicks  = m_intervalms / m_tickCounterPeriodMs;
    auto itTest         = m_intervalms * f_Hz_3; //This should remain quite similar to intervalTicks

    m_ticksPerInterval	= static_cast<Uint64>(intervalTicks);

}

void TimedEvent_::Reset(){

    //If running or stopped, continue running or stopped but if
    m_state = State::STOPPED;
    SDL_assert(m_ticksPerInterval);
    onReset.emit();

}

void TimedEvent_::Start(){

    switch(m_state){
        case State::STOPPED:
            m_t = *m_ptrTNow + m_ticksPerInterval;
            onStarted.emit();
            break;

        case State::PAUSED:
            m_t = *m_ptrTNow + m_intervalticksForPause;
            onStarted.emit();
            break;

        case State::RUNNING:
            m_t = *m_ptrTNow + m_intervalticksForLoop;
            break;

        default:
            break;
    }
    m_state = RUNNING;

}

void TimedEvent_::Stop(){

    Reset();
    onStopped.emit();

}

void TimedEvent_::RemainingTicksForLoop(uint64_t& refCounter){

	auto t_mod = (*m_ptrTNow) % m_ticksPerInterval;
    refCounter = m_ticksPerInterval - t_mod;

}

void TimedEvent_::Pause(){

	RemainingTicksForLoop(m_intervalticksForPause);
    m_state = State::PAUSED;
    onPaused.emit();

}

inline void TimedEvent_::TimerLoops(bool doLoop){

	m_timerLoops = doLoop;

}

void TimedEvent_::Update(){
    if (m_state == RUNNING){

		if (*m_ptrTNow >= m_t){

			onTimer.emit();

			if (m_timerLoops){

				RemainingTicksForLoop(m_intervalticksForLoop);
				//Start again.
				Start();

			} else {

				m_state = STOPPED;

			}

		}

	}

}

void TimedEvent_::SetCounterReference(Uint64 *ptr) {

    m_ptrTNow = ptr;

}
