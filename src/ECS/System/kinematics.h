#ifndef __KINEMATICS_H__
#define __KINEMATICS_H__

#include <ECS/Entity/entitymanager.h>

#include <ECS/Component/componentmanager.h>
#include <glm/vec3.hpp>
#include <set>

namespace ECS {

	class KinematicsSystem {


		static std::vector<unsigned int>	ids;
		static std::vector<glm::vec3*>		positions;
		static std::vector<glm::vec3*>		speeds;
		static std::vector<glm::vec3*>		accelerations;
		public:
			static unsigned int SubscribeEntity(unsigned int id){

				auto entityManager = ECS::EntityManager_::GetManager();
            	auto componentManager = ECS::ComponentManager_::GetManager();
            	auto componentVectors = entityManager->GetComponentsIds(id);

                using SetVarLambda      = std::function<void(unsigned int)>;

                using typeFunction = std::map<std::string, SetVarLambda >;

                ECS::SpeedComponent_* pSpeedComponent = nullptr;
                ECS::PositionComponent_* pPositionComponent = nullptr;
                ECS::AccelerationComponent_* pAccelerationComponent = nullptr;

                SetVarLambda getComponentAccelerationLambda = [&](unsigned int componentId) {

                    pAccelerationComponent = componentManager->GetComponentRaw<ECS::AccelerationComponent_>(componentId);
                    accelerations.push_back(&pAccelerationComponent->acceleration);

                };
                SetVarLambda getComponentPositionLambda     = [&](unsigned int componentId) {

                    pPositionComponent = componentManager->GetComponentRaw<ECS::PositionComponent_>(componentId);
                    positions.push_back(&pPositionComponent->position);

                };
                SetVarLambda getComponentSpeedLambda        = [&](unsigned int componentId) {

                    pSpeedComponent = componentManager->GetComponentRaw<ECS::SpeedComponent_>(componentId);
                    speeds.push_back(&pSpeedComponent->speed);

                };

                typeFunction mapFunc {
                        {"ECS::AccelerationComponent_", getComponentAccelerationLambda},
                        {"ECS::PositionComponent_",     getComponentPositionLambda},
                        {"ECS::SpeedComponent_",        getComponentSpeedLambda}
                };


            	for (auto& componentId:componentVectors){

                    auto key = componentManager->GetComponent(componentId)->GetType();
                	if (!mapFunc.count(key)) continue;
                    mapFunc[key](componentId);
                    mapFunc.erase(key);

	            }

	            if (mapFunc.size() > 0 || pSpeedComponent == nullptr || pPositionComponent == nullptr || pAccelerationComponent == nullptr){

	                for (auto key: mapFunc){
	                    std::cout << "RenderingSystem::SubscribeEntity : " << "Component not found: " << key.first << "\n";
	                }
                    if (pAccelerationComponent) accelerations.pop_back();
                    if (pPositionComponent) positions.pop_back();
                    if (pSpeedComponent) speeds.pop_back();
                    return 0;

	            }

	            //Position data
                ids.push_back(id);
                return 1;

			}
	};
}
#endif