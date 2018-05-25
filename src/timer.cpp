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

TimedEvent::TimedEvent(Uint32 delayMs, State action) {

    m_state = action;
    Set(delayMs);
    Reset();

    switch (m_state){
        case State::RUNNING: Start(); break;
        case State::PAUSED:
        case State::STOPPED:
        default:
            break;
    }

}

void TimedEvent::Set(Uint32 delayMs){

    m_intervalms = delayMs;

}

void TimedEvent::Reset(){

    //If running or stopped, continue running or stopped but if
    m_state = State::STOPPED;

    auto dtickperiodms  = 1000 / static_cast<double>(SDL_GetPerformanceFrequency());
    auto dintervalms    = m_intervalms / dtickperiodms;
    m_intervalticks     = static_cast<Uint64>(dintervalms);
    SDL_assert(m_intervalticks);

}

void TimedEvent::Start(){

    switch(m_state){

        case State::STOPPED:
            m_t = SDL_GetPerformanceCounter() + m_intervalticks;
            break;

        case State::PAUSED:
            m_t = SDL_GetPerformanceCounter() + m_intervalticksForPause;
            break;
        case State::RUNNING:
        default:
            break;
    }
    m_state = RUNNING;

}

void TimedEvent::Stop(){

    Reset();

}

void TimedEvent::Pause(){

    auto t_now = SDL_GetPerformanceCounter();
    auto t_delta = (t_now > m_t) ? 0 : m_t - t_now;
    m_intervalticksForPause = t_delta;
    m_state = State::PAUSED;

}

TimedEvent::operator bool() const {

    if (m_state != State::RUNNING) return false;

    auto t_now = SDL_GetPerformanceCounter();
    return t_now >= m_t;

}

unsigned int TimedEvent::RegisterSlot(std::function<void(shared_ptr<void>)> slot, shared_ptr<void> pParam = nullptr){

    Signal<shared_ptr<void>> signal;
    signal.connect(slot);
    auto lf = [=](){
        signal.emit(pParam);
    };
    unsigned int key = ++timedEventId;
    m_mActiveLambda[key] = ActiveFunctionPair{true, lf};
    return key;
}
/*
void DispatchTimerEvents(SDL_Event& rEvent){

    const auto ev   = rEvent.type;
    if (ev  != SDL_KEYDOWN && ev  != SDL_KEYUP) return;
    const auto key  = rEvent.key.keysym.sym;
    auto mask64     = static_cast<Uint64>(ev );
    mask64        <<= 32;
    mask64         |= key;
    auto beginmask  = vRegisteredKBEventsKeysMaskedPair.begin();
    auto endmask    = vRegisteredKBEventsKeysMaskedPair.end();
    auto it         = std::find(beginmask, endmask, mask64);
    if (it == endmask) return;

    auto vLambda    = mDispatchRegisteredLambda[mask64];
    for (auto& lambda : vLambda)
    {
        lambda();
    }
}
*/

