#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <cxxabi.h>
#include <SDL2/SDL_system.h>
#include "entity.h"
#include "Tech/G/Tech.h"


#include <glm/vec3.hpp>

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

        SDL_Texture* pTexture;

    public:
        SpriteComponent() {}


        inline SDL_Texture* GetTexture() {
            return pTexture;
        }

        inline void SetTexture(SDL_Texture* pt) {
            pTexture = pt;
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
    class AccelerationComponent : public VectorialComponent<double>{};

    using PSpeedComponent = std::shared_ptr<SpeedComponent>;
    using PAccelerationComponent = std::shared_ptr<AccelerationComponent>;

    using PSpeedComponent_ = std::weak_ptr<SpeedComponent>;
    using PAccelerationComponent_ = std::weak_ptr<AccelerationComponent>;

    class PositionComponent : public Component {
    public:
        glm::vec3 position;
    };

}


#endif /*__COMPONENT_H__*/