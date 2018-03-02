#ifndef __SHIPSPRITEFACTORY_H__
#define __SHIPSPRITEFACTORY_H__

#include "entitymanager.h"

using namespace GTech2D;
using namespace ECS;


namespace GAME {
    class ShipFactory {

        static void AddComponents(){

            //Add Sprite
            //Add Position
            //Add Acceleration

        }
    public:
        static unsigned int CreateShip(){

            EntityManager::UPEntityManager entityManager = EntityManager::GetManager();
            unsigned int shipId = entityManager->CreateEntity();

            return shipId;
        }

    };
}


#endif /* __SHIPSPRITEFACTORY_H__ */