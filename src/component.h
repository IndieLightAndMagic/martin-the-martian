#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <cxxabi.h>
#include "entity.h"
#include "Tech.h"

using namespace GTech;
class Entity;
namespace ECS {


    class Component {

    protected:
        unsigned int m_id;
        unsigned int m_parentId;

    public:
        Component() :
                m_parentId(0),
                m_id(0) {}

        friend class ComponentFactory;

        friend class ComponentManager;

        virtual std::string GetType() {
            auto name = typeid(*this).name();
            auto status = 4;
            std::unique_ptr<char, void (*)(void *)> res{
                    abi::__cxa_demangle(name, nullptr, nullptr, &status),
                    std::free
            };
            return (status == 0 ? res.get() : name);
        }
        virtual ~Component(){}
    };

    using ECSPComponent = std::shared_ptr<Component>;
    using ECSPComponent_ = std::weak_ptr<Component>;

    class SpriteComponent : public Component {

        GTech::GTPTexture2D texture;
        GTech::Vector2Dd anchor;
        double scale;
    public:
        SpriteComponent() : scale(1.0), anchor({0.0, 0.0}) {}

        GTech::Vector2Dd *GetAnchor() {
            return &anchor;
        }

        inline GTech::GTPTexture2D &GetTexture() {

            return texture;

        }

        inline double *GetScale() {

            return &scale;

        }

        inline void SetTexture(GTech::GTPTexture2D ptexture) {
            texture = ptexture;
        }

        inline void SetAnchor(const GTech::Vector2Dd &rAnchor) {

            anchor.x = rAnchor.x;

            anchor.y = rAnchor.y;
        }

        inline void SetScale(const double &rscale) {

            scale = rscale;

        }
    };


    template<typename T>
    class VectorialComponent : public Component {
    public:
        T x, y, z;
    };

    template<typename T>
    using PVectorialComponent = std::shared_ptr<VectorialComponent<T>>;


    class SpeedComponent : public VectorialComponent<double>{};
    class PositionComponent : public VectorialComponent<double>{};
    class AccelerationComponent : public VectorialComponent<double>{};

    using PSpeedComponent = std::shared_ptr<SpeedComponent>;
    using PPositionComponent = std::shared_ptr<PositionComponent>;
    using PAccelerationComponent = std::shared_ptr<AccelerationComponent>;

    using PSpeedComponent_ = std::weak_ptr<SpeedComponent>;
    using PPositionComponent_ = std::weak_ptr<PositionComponent>;
    using PAccelerationComponent_ = std::weak_ptr<AccelerationComponent>;
}

#endif /*__COMPONENT_H__*/