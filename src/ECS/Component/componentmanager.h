#ifndef __COMPONENT_MANAGER_H__
#define __COMPONENT_MANAGER_H__
#include <map>
#include <memory>
#include <iostream>
#include "component.h"
#include "ECS/Entity/entitymanager.h"
#include "componentfactory.h"
#include "entityinformationcomponent.h"
#include <memory>
#include <SDL2/SDL_assert.h>


namespace ECS{

    class EntityInformationComponent_;
    class ComponentManager {

        mutable std::map<unsigned int, Component>componentMap;

    public:
        static ComponentManager& GetInstance();

        template <typename T> unsigned int CreateComponent(){

            Component component = ECS::ComponentFactory::CreateComponent<T>();
            componentMap[component->m_id] = component;
            return component->m_id;

        }

        Component GetComponent(unsigned int componentId) const;

        template <typename T> T* GetComponentRaw(unsigned int componentId) const {


            auto ptr = dynamic_cast<T*>(GetComponent(componentId).get());
            SDL_assert(ptr);
            return ptr;

        }

        static const EntityInformationComponent_& GetInformationComponent(unsigned int entityId);



    };
    



}


#endif /* __COMPONENT_MANAGER_H__ */