#ifndef __BOLT__
#define __BOLT__

#include "entitymanager.h"
#include "ECS/Component/componentmanager.h"

namespace GAME {


	class Bolt {
	public:
		static unsigned int CreateBolt(){
			
			auto entityManager = ECS::EntityManager_::GetManager();
			auto componentManager = ECS::ComponentManager::GetManager();

			auto boltId = entityManager->CreateEntity();

			auto positionComponent = componentManager->CreateComponent<ECS::PositionComponent>();
			auto speedComponent = componentManager->CreateComponent<ECS::SpeedComponent>();

			std::string localPath = std::string(RES_DIR) + "orangebolt.png";
			SDL_Surface* pImageSurface = IMG_Load(localPath.c_str());
			if (!pImageSurface){
				std::cerr << "SDL Error loading surface from: " << localPath << "\n";
				SDL_assert(false);
			}

			SDL_Texture* pSDLTexture = SDLCreateTextureFromSurface(pImageSurface);
            auto spriteComponent = ECS::ComponentManager::GetManager()->CreateComponent<ECS::SpriteComponent>();
            auto rawSpriteComponent = dynamic_cast<ECS::SpriteComponent*>(ECS::ComponentManager::GetManager()->GetComponent(spriteComponent).get());
            rawSpriteComponent->SetTexture(pSDLTexture);

            entityManager->AddComponent(boltId, positionComponent);
            entityManager->AddComponent(boltId, speedComponent);
            entityManager->AddComponent(boltId, spriteComponent);

            return boltId;
		}

	};
}
#endif /* __BOLT__ */