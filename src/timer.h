#ifndef __TIMER_H__
#define __TIMER_H__

#include <SDL2/SDL.h>
#include <SignalSlot/signalslot.h>
#include <functional>
#include <vector>
#include <memory>
#include <map>

namespace ECS {
    class TimedEvent_;
    using TimedEvent = std::shared_ptr<TimedEvent_>;

    class TimedEvent_{
        /**
         * Next on timer event tick.
         */
        uint64_t m_t{0};
        /**
         * The size in ticks of the interval.
         */
        uint64_t m_ticksPerInterval{0};
        /**
         * Ticks left when pausing.
         */
        uint64_t m_intervalticksForPause{0};

        /**
         * Ticks left when pausing.
         */
        uint64_t m_intervalticksForLoop{0};

        /**
         * Timer size in ms.
         */
        uint32_t m_intervalms{0};


        enum State{

            RUNNING,
            STOPPED,
            PAUSED,

        };
        State m_state {STOPPED};


        /* A pointer to a reference Counter */

        uint64_t*   m_ptrTNow;
        /**
         * High Performance Counter Frequency (ticks per second)
         */
        static const uint64_t m_tickCounterFrequencyHz;

        /**
         * High Performance Counter Frequency (milliseconds per tick)
         */
        static const double_t m_tickCounterPeriodMs;

        /**
         * Does it the timer loop?
         */
        bool        m_timerLoops {false};

        /**
         *
         * @param refCounter Reference to high performance Counter.
         */
        void        RemainingTicksForLoop(uint64_t& refCounter);

    public:

        TimedEvent_(Uint32 delayMs = 0);
        void Reset();
        void Set(Uint32 intervalms);
        void Start();
        void Pause();
        void Stop();

        void SetCounterReference(Uint64* ptr);

        void TimerLoops(bool);


        void Update();

        GTech::Signal<> onTimer;
        GTech::Signal<> onReset;
        GTech::Signal<> onStarted;
        GTech::Signal<> onStopped;
        GTech::Signal<> onPaused;

    };
}


#endif //__TIMER_H__
