#ifndef __TIMER_H__
#define __TIMER_H__

#include <functional>
#include <vector>
#include <memory>
#include <map>

using ActiveFunctionPair = std::pair<bool, std::function<void()>>;
using ActiveFunctionPairMap = std::map<unsigned int, ActiveFunctionPair>;


class TimedEvent{
    Uint64 m_t;
    Uint64 m_intervalticks;
    Uint64 m_intervalticksForPause;
    Uint32 m_intervalms;
    enum State{
        RUNNING,
        STOPPED,
        PAUSED,
    };
    State m_state;
    ActiveFunctionPairMap m_mActiveLambda;
public:
    TimedEvent(Uint32 delayMs, State action = State::RUNNING);
    void Reset();
    void Set(Uint32 intervalms);
    void Start();
    void Pause();
    void Stop();
    explicit operator bool() const;
    unsigned int RegisterSlot(std::function<void(std::shared_ptr<void>)> slot, std::shared_ptr<void> pParam = nullptr);
};


#endif //__TIMER_H__

