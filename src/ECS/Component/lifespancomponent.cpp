#include "component.h"

namespace ECS {

    static Uint64 reference = SDL_GetPerformanceCounter();
    void LifeSpanComponent_::Set(float ms)
	{
	    m_te.Set(ms);
        m_te.onTimer.connect_signal(onLifeSpanEnded);
        m_te.SetCounterReference(&ECS::reference);
        m_te.Start();
	}

    void LifeSpanComponent_::Update()
    {
        ECS::reference = SDL_GetPerformanceCounter();
        m_te.Update();
    }
}