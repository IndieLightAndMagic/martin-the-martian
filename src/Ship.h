#ifndef __SHIP__
#define __SHIP__

#include <set>
#include <SDL2_image/SDL_image.h>

#include "Tech/G/Tech.h"
#include "ECS/Entity/entitymanager.h"
#include "ECS/Component/componentmanager.h"

SDL_Texture* SDLCreateTextureFromSurface(SDL_Surface* pSurface);
namespace GAME {


    class Ship {

    public:
        static unsigned int CreateShip(std::string path){

            auto entityManager = ECS::EntityManager_::GetManager();
            auto componentManager = ECS::ComponentManager_::GetManager();

            //Create Ship Entity_a
            auto shipId = entityManager->CreateEntity();

            //Create Texture Id
            auto textureComponentId = componentManager->CreateComponent<ECS::TextureComponent_>();
            auto textureComponentRP = componentManager->GetComponentRaw<ECS::TextureComponent_>(textureComponentId);
            //SetTexture
            textureComponentRP->SetTexture(path);

            //Add Components to shipId
            entityManager->AddComponent(shipId, componentManager->CreateComponent<ECS::PositionComponent_>());
            entityManager->AddComponent(shipId, componentManager->CreateComponent<ECS::SpeedComponent_>());
            entityManager->AddComponent(shipId, textureComponentId);

            return shipId;

        }


        static void SetShipPosition(unsigned int shipId, int x, int y) {

            auto componentsList = ECS::EntityManager_::GetManager()->GetComponentsIds(shipId);
            auto positionComponent = dynamic_cast<ECS::PositionComponent_*>(ECS::ComponentManager_::GetManager()->GetComponent(componentsList[0]).get());
            positionComponent->position = glm::vec3(x,y,0) ;

        }

        static void SetShipPositionDelta(unsigned int shipId, int x, int y) {

            auto componentsList = ECS::EntityManager_::GetManager()->GetComponentsIds(shipId);
            auto genericComponent = ECS::ComponentManager_::GetManager()->GetComponent(componentsList[0]);

            auto rawPointerGenericComponent = genericComponent.get();
            auto pointerPositionComponent   = dynamic_cast<ECS::PositionComponent_*>(rawPointerGenericComponent);

            pointerPositionComponent->position.x += x;
            pointerPositionComponent->position.y += y;

        }



    };
}


#endif /* __SHIP__ */