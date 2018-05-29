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
        TextureComponent_(std::string path) {
            SetTexture(path.c_str());
        }
        TextureComponent_(const char* path) {
            SetTexture(path);
        }

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

    class SpeedComponent_;
    using SpeedComponent = std::shared_ptr<SpeedComponent_>;
    class SpeedComponent_ : public Component_{
    public:
        glm::vec3 speed{0.0f, 0.0f, 0.0f};
    };

    class AccelerationComponent_;
    using AccelerationComponent = std::shared_ptr<AccelerationComponent_>;
    class AccelerationComponent_ : public Component_{
    public:
        glm::vec3 acceleration{0.0f, 0.0f, 0.0f};
    };

    class PositionComponent_;
    using PositionComponent = std::shared_ptr<PositionComponent_>;
    class PositionComponent_ : public Component_ {
    public:
        glm::vec3 position{0.0f, 0.0f, 0.0f};
    };

}


#endif /*__COMPONENT_H__*/