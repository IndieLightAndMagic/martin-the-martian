#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "entityfactory.h"
#include <map>

namespace ECS {

    class EntityManager{

        std::map<unsigned int, ECSPEntity>entityMap;

    public:
        using UPEntityManager = std::shared_ptr<EntityManager>;
        static UPEntityManager entityManager;

        static UPEntityManager& GetManager(){
            if ( entityManager == nullptr ){
                entityManager = std::make_unique<EntityManager>();
            }
            return entityManager;
        }

        unsigned int CreateEntity(){
            ECSPEntity pEntity = EntityFactory::CreateEntity();
            entityMap[pEntity->m_id] = pEntity;
            return pEntity->m_id;
        }

        bool AddComponent(unsigned int entity, unsigned int componentId){

            entityMap[entity]->m_componentIds.push_back(componentId);
            return true;

        }

        std::vector<unsigned int>& GetComponentIds(unsigned int entity){
            return entityMap[entity]->m_componentIds;
        }
        ECSPEntity GetEntity(unsigned int entity){
            return entityMap[entity];
        }


    };
    EntityManager::UPEntityManager EntityManager::entityManager = nullptr;

}

#endif /* __ENTITYMANAGER_H__ */

