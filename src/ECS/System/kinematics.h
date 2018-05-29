#ifndef __KINEMATICS_H__
#define __KINEMATICS_H__

#include <timer.h>
#include <ECS/Entity/entitymanager.h>
#include <ECS/Component/componentmanager.h>
#include <ECS/Component/entityinformationcomponent.h>

#include <glm/vec3.hpp>
#include <set>

//Constant aceleration kinematic model.

namespace ECS {

	class KinematicsSystem {


		static std::vector<unsigned int>	ids;
		static std::vector<glm::vec3*>		positions;
		static std::vector<glm::vec3*>		speeds;
		static std::vector<glm::vec3*>		accelerations;
        static glm::vec3**                  positions_;
        static glm::vec3**                  speeds_;
        static glm::vec3**                  accelerations_;
        static uint64_t                     dtTicks;
        static bool                         firstTime;

    public:
            static void InitKinematicsSystem(){
                firstTime = true;

            }

            static unsigned int SubscribeEntity(unsigned int id){

                auto entityManager      = ECS::EntityManager_::GetManager();
                auto componentManager   = ECS::ComponentManager_::GetManager();
                auto componentVectors   = entityManager->GetComponentsIds(id);

                //CONVENTION: the first component of ANY entity is the EntityInformationComponent_
                auto entityInfo         = componentVectors[0];
                auto entityInfoRP       = componentManager->GetComponentRaw<ECS::EntityInformationComponent_>(entityInfo);

                //Kinematic Triad
                auto [posId, speedId, accelId]                      = entityInfoRP->GetKinematicTupleIds();

                auto pSpeedComponent            = componentManager->GetComponentRaw<ECS::SpeedComponent_>(speedId);
                auto pPositionComponent         = componentManager->GetComponentRaw<ECS::PositionComponent_>(posId);
                auto pAccelerationComponent     = componentManager->GetComponentRaw<ECS::PositionComponent_>(accelId);


	            //Position data
                ids.push_back(id);

                accelerations_  = accelerations.data();
                speeds_         = speeds.data();
                positions_      = positions.data();

                return 1;

			}
            static void UpdateKinematicsSystem(){

                //x(t) = 0.5a(t)**2 + vo(t) + xo
                //s(t) = a(t) + vo
                //a(t)
                if (firstTime){
                    dtTicks = SDL_GetPerformanceCounter();
                    firstTime = false;
                    return;
                }
                dtTicks = SDL_GetPerformanceCounter() - dtTicks;
                float dtSecs = 1.0f / SDL_GetPerformanceFrequency(); //How long does it take to one tick
                dtSecs *= dtTicks;

                auto sz = ids.size();
                auto dtSecs2    = dtSecs * dtSecs;
                auto kinematic  = [&](int index){

                    auto accel  = accelerations_[index];
                    auto speed  = speeds_[index];
                    auto& pos   = *(positions_[index]);
                    glm::vec3 accelDelta(0.5 * accel->x * dtSecs2, 0.5 * accel->y * dtSecs2, 0.5 * accel->z * dtSecs2);
                    glm::vec3 speedDelta(speed->x * dtSecs, speed->y * dtSecs, speed->z * dtSecs);
                    pos += accelDelta + speedDelta;

                };

                for (unsigned int i = 0; i < sz; ++i) kinematic(i);

            }
	};
}
#endif