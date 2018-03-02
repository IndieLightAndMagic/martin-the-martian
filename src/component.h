#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "entity.h"
#include "Tech.h"

using namespace GTech2D;
class Entity;
namespace ECS {

    struct Component
    {
        static unsigned int unique_component_id;
        enum struct ComponentType { NONE, CHILD_ENTITY, SPRITE, POSITION, SPEED, DIRECTION, ACCELERATION, CONTROL_INPUT};
        UPEntity m_pParent;
        unsigned int m_id;
        ComponentType m_type;
        int SetParent(UPEntity m_parent);
        Component():
                m_pParent(UPEntity(nullptr)),
                m_id(++unique_component_id),
                m_type(NONE)
        {}

    };
    using UPComponent = std::unique_ptr<Component>;
    unsigned int Component::unique_component_id = 0;
    struct SpriteComponent : Component {

        UPTexture2D texture;
        Vector2Dd anchor;
        double scale;

        SpriteComponent(unsigned int parentId):
                anchor(Vector2Dd{0.5f, 0.5f}),
        {
            m_pParent = parent;
            m_type = Component::ComponentType::SPRITE;
        }

    };
    using UPSpriteComponent = std::unique_ptr<SpriteComponent>;
    struct Vector2Dd: Component, Vector2Df {

    };


}


#endif /*__COMPONENT_H__*/