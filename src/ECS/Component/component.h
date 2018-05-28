#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <cxxabi.h>
#include <SDL2/SDL_system.h>
#include "ECS/Entity/entity.h"
#include "Tech/G/Tech.h"


#include <glm/vec3.hpp>

SDL_Texture* SDLCreateTextureFromFile(const char *path);

using namespace GTech;
namespace ECS {


    class Component_ {

    protected:
        unsigned int m_id;
        unsigned int m_parentId;

    public:
        Component_(const Component_&) = default;
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


    class TextureComponent_;
    using TextureComponent = std::shared_ptr<TextureComponent_>;
    class TextureComponent_ : public Component_ {

        SDL_Texture* m_pTexture;

    public:
        TextureComponent_() {}

        inline SDL_Texture* GetTexture() {
            return m_pTexture;
        }

        inline void SetTexture(SDL_Texture* pt) {
            m_pTexture = pt;
        }

        inline void SetTexture(const char* path) {
            SetTexture(SDLCreateTextureFromFile(path));
        }
        inline void SetTexture(std::string path) {
            SetTexture(path.c_str());
        }

    };


    template<typename T>
    class VectorialComponent : public Component_ {
    public:
        T x, y, z;
    };

    template<typename T>
    using PVectorialComponent = std::shared_ptr<VectorialComponent<T>>;

    class SpeedComponent_;
    using SpeedComponent = std::shared_ptr<SpeedComponent_>;
    class SpeedComponent_ : public VectorialComponent<double>{};
    class AccelerationComponent : public VectorialComponent<double>{};

    using PSpeedComponent = std::shared_ptr<SpeedComponent_>;
    using PAccelerationComponent = std::shared_ptr<AccelerationComponent>;

    using PSpeedComponent_ = std::weak_ptr<SpeedComponent_>;
    using PAccelerationComponent_ = std::weak_ptr<AccelerationComponent>;

    class PositionComponent_;
    using PositionComponent = std::shared_ptr<PositionComponent_>;
    class PositionComponent_ : public Component_ {
    public:
        glm::vec3 position;
    };

}


#endif /*__COMPONENT_H__*/