#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <cxxabi.h>
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
        virtual std::string GetType(){
            auto name = typeid(*this).name();
            auto status = 4;
            std::unique_ptr<char, void(*)(void*)> res {
                    abi::__cxa_demangle(name, nullptr, nullptr, &status),
                    std::free
            };
            return (status == 0 ? res.get() : name);
        }
    };
    using ECSPComponent = std::shared_ptr<Component>;
    using ECSPComponent_ = std::weak_ptr<Component>;

    class SpriteComponent: public Component {

        GTech2D::GTPTexture2D texture;
        GTech2D::Vector2Dd anchor;
        double scale;
    public:
        SpriteComponent():scale(1.0), anchor({0.0,0.0}){}
        GTech2D::Vector2Dd* GetAnchor(){
            return &anchor;
        }
        inline GTech2D::GTPTexture2D& GetTexture(){
            return texture;
        }
        inline double* GetScale(){
            return &scale;
        }

        inline void SetTexture(GTech2D::GTPTexture2D ptexture){
            texture = ptexture;
        }
    };

    class Vector2DdComponent : public Component{
    protected:
        GTech2D::Vector2Dd v;
    public:
        GTech2D::Vector2Dd* Get() { return &v; }
    };

    class SpeedComponent : public Vector2DdComponent{

    };

    class PositionComponent : public Vector2DdComponent{

    };

    class AccelerationComponent : public Vector2DdComponent{

    };
}


#endif /*__COMPONENT_H__*/