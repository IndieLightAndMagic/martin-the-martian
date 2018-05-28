#ifndef __BOLT__
#define __BOLT__

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
			auto positionComponent  = componentManager->CreateComponent<ECS::PositionComponent_>();
			auto speedComponent     = componentManager->CreateComponent<ECS::SpeedComponent_>();

			//Create Texture Id
			auto textureComponentId = componentManager->CreateComponent<ECS::TextureComponent_>();
			auto textureComponentRP = componentManager->GetComponentRaw<ECS::TextureComponent_>(textureComponentId);
			//SetTexture
			textureComponentRP->SetTexture(path);

			//Add Components to boltId
			entityManager->AddComponent(boltId, positionComponent);
			entityManager->AddComponent(boltId, speedComponent);
			entityManager->AddComponent(boltId, textureComponentId);

			return boltId;
		}

	};
}
#endif /* __BOLT__ */