#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "entity.h"
#include "Tech.h"

using namespace GTech2D;
class Entity;
namespace ECS {
    template <typename T>
    class Component
    {
    public:
        int SetParent(unsigned int parentId);
        unsigned int m_id;
        T m_data;
    private:
        unsigned int m_parentId;

    public:
        Component():
                m_parentId(0),
                m_id(0)
        {}

    };
    template <typename T>
    using ECSPComponent<T> = std::shared_ptr<Component<T>>;

    class SpriteComponent {

        GTech2D::ECSPTexture2D texture;
        inline GTech2D::Vector2Dd anchor;
        double scale;
    public:
        GTech2D::Vector2Dd& GetAnchor(){
            return anchor;
        }
        inline GTech2D::ECSPTexture2D& GetTexture(){
            return texture;
        }
        inline double& GetScale(){
            return scale;
        }

    };
    using ECSPSpriteComponent = std::shared_ptr<SpriteComponent>;

    class Vector2DdComponent : public Component, GTech2D::Vector2Dd{

    };
    using SpeedComponent = Vector2DdComponent;
    using PositionComponent = Vector2DdComponent;
    using AccelerationComponent = Vector2DdComponent;
}


#endif /*__COMPONENT_H__*/