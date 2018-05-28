#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "entityfactory.h"
#include <map>
#include <memory>

namespace ECS {
	class EntityManager_;
	using EntityManager = std::shared_ptr<EntityManager_>;
    class EntityManager_{

        std::map<unsigned int, Entity>entityMap;

    public:

        static EntityManager entityManager;
        static EntityManager& GetManager();

        unsigned int CreateEntity();
		bool AddComponent(unsigned int entity, unsigned int componentId);

        std::vector<unsigned int>& GetComponentsIds(unsigned int entity);
        Entity GetEntity(unsigned int entity);

    };

}

#endif /* __ENTITYMANAGER_H__ */

