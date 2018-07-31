#include "component.h"

namespace ECS {

    Uint64 LifeSpanComponent_::lifeSpanCounter = SDL_GetPerformanceCounter();
	void LifeSpanComponent_::Set(float ms)
	{
        m_te.Set(ms);
        m_te.SetCounterReference(&lifeSpanCounter);
        m_te.onTimer.connect_signal(onLifeSpanEnded);
	}


}