#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <cxxabi.h>
#include <SDL2/SDL_system.h>
#include "ECS/Entity/entity.h"
#include "Tech/G/Tech.h"


#include <glm/vec3.hpp>

using namespace GTech;
namespace ECS {


    class Component_ {

    protected:
        unsigned int m_id;
        unsigned int m_parentId;

    public:
        Component_() :
                m_parentId(0),
                m_id(0) {}

        friend class ComponentFactory;

        friend class ComponentManager_;

        virtual std::string GetType() {
            auto name = typeid(*this).name();
            auto status = 4;
            std::unique_ptr<char, void (*)(void *)> res{
                    abi::__cxa_demangle(name, nullptr, nullptr, &status),
                    std::free
            };
            return (status == 0 ? res.get() : name);
        }
        virtual ~Component_(){}
    };

    using Component = std::shared_ptr<Component_>;
    using _Component = std::weak_ptr<Component_>;

    class SpriteComponent : public Component_ {

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
    class VectorialComponent : public Component_ {
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

    class PositionComponent : public Component_ {
    public:
        glm::vec3 position;
    };

}


#endif /*__COMPONENT_H__*/