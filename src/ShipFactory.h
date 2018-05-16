#ifndef __SHIPSPRITEFACTORY_H__
#define __SHIPSPRITEFACTORY_H__

#include <set>

#include "Tech/G/Tech.h"
#include "entitymanager.h"
#include "ECS/Component/componentmanager.h"

namespace GAME {


    class ShipFactory {

    public:
        static unsigned int CreateShip(const GTech::Tech &rptech) {

            auto entityManager = ECS::EntityManager::GetManager();
            auto componentManager = ECS::ComponentManager::GetManager();

            //Create Ship Entity
            auto shipId = entityManager->CreateEntity();

            //Create a Position & Speed Components
            auto positionComponent = componentManager->CreateComponent<ECS::PositionComponent>();
            auto speedComponent = componentManager->CreateComponent<ECS::SpeedComponent>();

            auto shipTexture = rptech->LoadTexture("hero.png");

            auto spriteComponent = componentManager->CreateComponent<ECS::SpriteComponent>();
            auto spSpriteComponent = componentManager->GetComponent(spriteComponent);
            auto rpSpriteComponent = static_cast<ECS::SpriteComponent *>(spSpriteComponent.get());

            rpSpriteComponent->SetTexture(shipTexture);
            rpSpriteComponent->SetAnchor({0.5, 0.5});


            //Add Components to ship
            entityManager->AddComponent(shipId, positionComponent);
            entityManager->AddComponent(shipId, speedComponent);
            entityManager->AddComponent(shipId, spriteComponent);


            return shipId;
        }

        static void SetShipPosition(unsigned int shipId, int x, int y) {

            auto componentsList = ECS::EntityManager::GetManager()->GetComponentIds(shipId);
            auto genericComponent = ECS::ComponentManager::GetManager()->GetComponent(componentsList[0]);
            auto rawPointerGenericComponent = genericComponent.get();
            auto pointerPositionComponent = dynamic_cast<ECS::PositionComponent *>(rawPointerGenericComponent);
            pointerPositionComponent->x = x;
            pointerPositionComponent->y = y;
            pointerPositionComponent->z = 0;

        }
    };
}


#endif /* __SHIPSPRITEFACTORY_H__ */