#ifndef __TIMEDEVENTS_H__
#define __TIMEDEVENTS_H__

#include <timer.h>
#include <ECS/Entity/entitymanager.h>
#include <ECS/Component/componentmanager.h>
#include <ECS/Component/entityinformationcomponent.h>

#include <glm/vec3.hpp>
#include <set>

//Constant aceleration kinematic model.

namespace ECS {

    class TimedEventsSystem {
        static Uint64 timerReference;

        static std::vector<unsigned int>                        entitiesIs;
        static std::vector<std::vector<LifeSpanComponent_*>>    positions;
        
    public:
         static void InitTimedEventsSystem();
         static unsigned int SubscribeEntity(unsigned int entityId);
         static void UpdateTimedEventsSystem();

    };
}
#endif