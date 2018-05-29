#ifndef __BOLT__
#define __BOLT__

#include <ECS/Component/entityinformationcomponent.h>
#include "ECS/Entity/entitymanager.h"
#include "ECS/Component/componentmanager.h"

namespace GAME {


	class Bolt {
	public:
		static unsigned int CreateBolt(std::string& path){

			auto entityManager = ECS::EntityManager_::GetManager();
			auto componentManager = ECS::ComponentManager_::GetManager();

			//Create Ship Entity_a
			auto boltId = entityManager->CreateEntity();

			//Create a Position & Speed Components
			auto accelerationComponent  = componentManager->CreateComponent<ECS::AccelerationComponent_>();
            auto positionComponent      = componentManager->CreateComponent<ECS::PositionComponent_>();
			auto speedComponent         = componentManager->CreateComponent<ECS::SpeedComponent_>();

            //Set information tuples for Systems
            auto infoComponent          = componentManager->CreateComponent<ECS::EntityInformationComponent_>();
            auto infoComponentRP        = componentManager->GetComponentRaw<ECS::EntityInformationComponent_>(infoComponent);
            infoComponentRP->SetKinematicTupleIds(positionComponent, speedComponent, accelerationComponent);

            //Create Texture Id
			auto textureComponentId     = componentManager->CreateComponent<ECS::TextureComponent_>();
			auto textureComponentRP     = componentManager->GetComponentRaw<ECS::TextureComponent_>(textureComponentId);

            //SetTexture
			textureComponentRP->SetTexture(path);

			//Add Components to boltId
			entityManager->AddComponent(boltId, infoComponent);
            entityManager->AddComponent(boltId, accelerationComponent);
            entityManager->AddComponent(boltId, positionComponent);
			entityManager->AddComponent(boltId, speedComponent);
			entityManager->AddComponent(boltId, textureComponentId);

			return boltId;

		}

	};
}
#endif /* __BOLT__ */