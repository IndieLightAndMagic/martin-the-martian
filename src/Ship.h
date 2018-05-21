#ifndef __SHIP__
#define __SHIP__

#include <set>
#include <SDL2_image/SDL_image.h>

#include "Tech/G/Tech.h"
#include "entitymanager.h"
#include "ECS/Component/componentmanager.h"

SDL_Texture* SDLCreateTextureFromSurface(SDL_Surface* pSurface);
namespace GAME {


    class Ship {

    public:
        static unsigned int CreateShip() {

            auto entityManager = ECS::EntityManager_::GetManager();
            auto componentManager = ECS::ComponentManager::GetManager();

            //Create Ship Entity
            auto shipId = entityManager->CreateEntity();

            //Create a Position & Speed Components
            auto positionComponent = componentManager->CreateComponent<ECS::PositionComponent>();
            auto speedComponent = componentManager->CreateComponent<ECS::SpeedComponent>();

            std::string localPath = std::string(RES_DIR) + "hero.png";
            SDL_Surface* pImageSurface = IMG_Load(localPath.c_str());
            if (!pImageSurface){
                std::cerr << "SDL Error loading surface from: " << localPath << "\n";
                SDL_assert(false);
            }

            SDL_Texture* pSDLTexture = SDLCreateTextureFromSurface(pImageSurface);
            auto spriteComponent = ECS::ComponentManager::GetManager()->CreateComponent<ECS::SpriteComponent>();
            auto rawSpriteComponent = dynamic_cast<ECS::SpriteComponent*>(ECS::ComponentManager::GetManager()->GetComponent(spriteComponent).get());
            rawSpriteComponent->SetTexture(pSDLTexture);

            //Add Components to ship
            entityManager->AddComponent(shipId, positionComponent);
            entityManager->AddComponent(shipId, speedComponent);
            entityManager->AddComponent(shipId, spriteComponent);


            return shipId;
        }

        static void SetShipPosition(unsigned int shipId, int x, int y) {

            auto componentsList = ECS::EntityManager_::GetManager()->GetComponentIds(shipId);
            auto genericComponent = ECS::ComponentManager::GetManager()->GetComponent(componentsList[0]);

            auto rawPointerGenericComponent = genericComponent.get();
            auto pointerPositionComponent   = dynamic_cast<ECS::PositionComponent*>(rawPointerGenericComponent);

            pointerPositionComponent->position = glm::vec3(x,y,0) ;

        }
    };
}


#endif /* __SHIP__ */