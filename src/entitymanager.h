#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "entityfactory.h"
#include <map>

namespace ECS {

    class EntityManager_{

        std::map<unsigned int, ECSPEntity>entityMap;

    public:
        using ECSPEntityManager = std::shared_ptr<EntityManager_>;
        static ECSPEntityManager entityManager;

        static ECSPEntityManager& GetManager(){
            if ( entityManager == nullptr ){
                entityManager = std::make_unique<EntityManager_>();
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
    EntityManager_::ECSPEntityManager EntityManager_::entityManager = nullptr;

}

#endif /* __ENTITYMANAGER_H__ */

