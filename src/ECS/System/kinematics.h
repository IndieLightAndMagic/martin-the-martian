#ifndef __KINEMATICS_H__
#define __KINEMATICS_H__

#include <ECS/Entity/entitymanager.h>

#include <ECS/Component/componentmanager.h>
#include <glm/vec3.hpp>

namespace ECS {

	class KinematicsSystem {


		static std::vector<unsigned int>	ids;
		static std::vector<glm::vec3&>		positions;
		static std::vector<glm::vec3&>		speeds;
		static std::vector<glm::vec3&>		accelerations;
		public:
			static unsigned int SubscribeEntity(unsigned int id){

				auto entityManager = ECS::EntityManager_::GetManager();
            	auto componentManager = ECS::ComponentManager_::GetManager();
            	auto componentVectors = entityManager->GetComponentsIds(id);

            	std::set<std::string> compotypes{"ECS::PositionComponent_","ECS::SpeedComponent_","ECS::AccelerationComponent_"};

            	ECS::SpeedComponent_* pSpeedComponent = nullptr;
            	ECS::PositionComponent_* pPositionComponent = nullptr;
            	ECS::AccelerationComponent_* pAccelerationComponent = nullptr;
            

            	for (auto& componentId:componentVectors){

                	auto pComponent = componentManager->GetComponent(componentId);
                	auto pComponentType = pComponent->GetType();

	                //Search
	                auto pit = compotypes.find(pComponentType);
	                if (pit == compotypes.end()) continue;

	                //Found & erase
	                compotypes.erase(pit);

	                auto rawPtr = pComponent.get();
	                if (pComponentType == "ECS::PositionComponent_")     pPositionComponent = componentmanager->GetComponent
	                else if (pComponentType == "ECS::SpeedComponent_")  	pSpriteComponent = dynamic_cast<TextureComponent_*>(pComponent.get());
	                else if (pComponentType == "ECS::AccelerationComponent_")  	pAccelerationComponent = dynamic_cast<TextureComponent_*>(pComponent.get());

	            }

	            if (compotypes.size() > 0 || pSpriteComponent == nullptr || pPositionComponent == nullptr){

	                for (auto &type: compotypes){
	                    std::cout << "RenderingSystem::SubscribeEntity : " << "Component not found: " << type << "\n";
	                }
	                return 0;

	            }

	            auto pTexture = pSpriteComponent->GetTexture();


	            //Texture Data
	            textures.push_back(pTexture);
	            int w,h;
	            SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);

	            auto sizevec = glm::ivec2(w,h);
	            textureSizes.push_back(sizevec);

	            //Position data
	            positions.push_back(&pPositionComponent->position);

	            ids.push_back(id);

	            return 1;

			}
	}
}
#endif