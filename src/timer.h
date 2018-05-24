#ifndef __TIMER_H__
#define __TIMER_H__

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

public:
    TimedEvent(Uint32 delayMs, State action = State::RUNNING);
    void Reset();
    void Set(Uint32 intervalms);
    void Start();
    void Pause();
    void Stop();
    explicit operator bool() const;

};


#endif //__TIMER_H__

