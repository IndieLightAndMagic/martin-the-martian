#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "entity.h"
#include "Tech.h"

using namespace GTech2D;
class Entity;
namespace ECS {
    class Component
    {
    protected:
        unsigned int m_id;
        unsigned int m_parentId;

    public:
        Component():
                m_parentId(0),
                m_id(0)
        {}
        friend class ComponentFactory;
        friend class ComponentManager;
    };
    using ECSPComponent = std::shared_ptr<Component>;

    class SpriteComponent: public Component {

        GTech2D::GTPTexture2D texture;
        GTech2D::Vector2Dd anchor;
        double scale;
    public:
        GTech2D::Vector2Dd& GetAnchor(){
            return anchor;
        }
        inline GTech2D::GTPTexture2D& GetTexture(){
            return texture;
        }
        inline double& GetScale(){
            return scale;
        }

    };

    class Vector2DdComponent : public Component{
    public:
        GTech2D::Vector2Dd v;
    };
    using SpeedComponent = Vector2DdComponent;
    using PositionComponent = Vector2DdComponent;
    using AccelerationComponent = Vector2DdComponent;
}


#endif /*__COMPONENT_H__*/