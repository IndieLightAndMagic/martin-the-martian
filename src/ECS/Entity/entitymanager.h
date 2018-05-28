#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "entityfactory.h"
#include <map>

namespace ECS {

    class EntityManager_{

        std::map<unsigned int, Entity>entityMap;

    public:
        using ECSPEntityManager = std::shared_ptr<EntityManager_>;
        static ECSPEntityManager entityManager;

        static ECSPEntityManager& GetManager();
        unsigned int CreateEntity();
		bool AddComponent(unsigned int entity, unsigned int componentId);
		std::vector<unsigned int>& GetComponentIds(unsigned int entity);
        Entity GetEntity(unsigned int entity);
        


    };

}

#endif /* __ENTITYMANAGER_H__ */

