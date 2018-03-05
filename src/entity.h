#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "component.h"

#include <vector>
#include <memory>
class Component;

namespace ECS {

    class Entity {

    public:
        unsigned int m_id;
        std::vector<unsigned int> m_componentIds;
    };
    using ECSPEntity = std::shared_ptr<Entity>;

}

#endif /*__ENTITY_H__*/

