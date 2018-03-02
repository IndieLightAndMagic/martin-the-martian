#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "entityfactory.h"
#include <map>

namespace ECS {

    class EntityManager{

        using UPEntityManager = std::unique_ptr<EntityManager>;
        std::map<unsigned int, ECSPEntity>entityMap;

    public:

        static UPEntityManager entityManager;
        static UPEntityManager GetManager(){
            if ( entityManager == nullptr ){
                entityManager = std::make_unique<EntityManager>(new EntityManager());
            }
            return entityManager;
        }

        unsigned int CreateEntity(){
            ECSPEntity pEntity = EntityFactory::CreateEntity();
            entityMap[pEntity->m_id] = pEntity;
            return pEntity->m_id;
        }

    };
    EntityManager::UPEntityManager EntityManager::entityManager = nullptr;

}

#endif /* __ENTITYMANAGER_H__ */

