#ifndef __COMPONENT_MANAGER_H__
#define __COMPONENT_MANAGER_H__
#include <map>
#include <memory>
#include <iostream>
#include "component.h"
#include "ECS/Entity/entitymanager.h"
#include "componentfactory.h"
#include <memory>
namespace ECS{

    class ComponentManager_ {

        using ComponentManager = std::shared_ptr<ComponentManager_>;
        std::map<unsigned int, Component>componentMap;

    public:
        static ComponentManager componentManager;
        static ComponentManager GetManager();
        
        template <typename T>
        unsigned int CreateComponent(){

            Component component = ECS::ComponentFactory::CreateComponent<T>();
            componentMap[component->m_id] = component;
            return component->m_id;

        }
        inline Component GetComponent(unsigned int componentId) {

            return componentMap[componentId];

        }
		template <typename T>
        inline T* GetComponentRaw(unsigned int componentId){
            return dynamic_cast<T*>(componentManager->GetComponent(componentId).get());
        }
    };
    



}


#endif /* __COMPONENT_MANAGER_H__ */