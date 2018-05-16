#ifndef __COMPONENT_MANAGER_H__
#define __COMPONENT_MANAGER_H__
#include <map>
#include <memory>
#include <iostream>
#include "component.h"
#include "entitymanager.h"
#include "componentfactory.h"

namespace ECS{

    class ComponentManager {

        using ECSPComponentManager = std::shared_ptr<ComponentManager>;
        std::map<unsigned int, ECSPComponent>componentMap;

    public:
        static ECSPComponentManager componentManager;
        static ECSPComponentManager GetManager();
        
        template <typename T>
        unsigned int CreateComponent(){

            ECSPComponent component = ECS::ComponentFactory::CreateComponent<T>();
            componentMap[component->m_id] = component;
            return component->m_id;

        }
        ECSPComponent GetComponent(unsigned int componentId);
		
    };
    



}


#endif /* __COMPONENT_MANAGER_H__ */