#ifndef __SHIPSPRITEFACTORY_H__
#define __SHIPSPRITEFACTORY_H__

#include "Tech.h"
#include "entitymanager.h"
#include "componentmanager.h"

namespace GAME {

    class Ship

    class ShipFactory {

    public:
        static unsigned int CreateShip(){

            ECS::EntityManager::UPEntityManager entityManager = ECS::EntityManager::GetManager();
            unsigned int shipId = entityManager->CreateEntity();

            //Create a Position
            auto componentManager = ECS::ComponentManager::GetManager();
            auto positionComponent = componentManager->CreateComponent<GTech2D::Vector2Dd>();
            auto speedComponent = componentManager->CreateComponent<GTech2D::Vector2Dd>();


            return shipId;
        }
    };


}


#endif /* __SHIPSPRITEFACTORY_H__ */