#ifndef __SHIPSPRITEFACTORY_H__
#define __SHIPSPRITEFACTORY_H__

#include "Tech.h"
#include "entitymanager.h"
#include "componentmanager.h"

namespace GAME {



    class ShipFactory {

    public:
        static unsigned int CreateShip(){

            auto entityManager = ECS::EntityManager::GetManager();
            auto componentManager = ECS::ComponentManager::GetManager();

            //Create Ship Entity
            auto shipId = entityManager->CreateEntity();

            //Create a Position & Speed Components
            auto positionComponent = componentManager->CreateComponent<ECS::PositionComponent>();
            auto speedComponent = componentManager->CreateComponent<ECS::SpeedComponent>();
            auto spriteComponent = componentManager->CreateComponent<ECS::SpriteComponent>();

            //Add Components to ship
            entityManager->AddComponent(shipId, positionComponent);
            entityManager->AddComponent(shipId, speedComponent);
            entityManager->AddComponent(shipId, spriteComponent);

            return shipId;
        }
    };

    class RenderingSystem {
    public:
        static unsigned int SubscribeEntity(unsigned int id){

            //Get Manager
            auto entityManager = ECS::EntityManager::GetManager();
            auto componentManager = ECS::ComponentManager::GetManager();

            auto componentVectors = entityManager->GetComponentIds(id);

            //Vector of components of this entity
            for (auto& componentId:componentVectors){

                auto pComponent = componentManager->GetComponent(componentId);
                std::cout << "Component Type: " << pComponent->GetType() << "\n";

            }

            return 0;

        }
    };

}


#endif /* __SHIPSPRITEFACTORY_H__ */