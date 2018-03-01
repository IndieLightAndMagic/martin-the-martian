#ifndef __COMPONENT_MANAGER_H__
#define __COMPONENT_MANAGER_H__
#include <map>
#include <memory>
#include "component.h"

namespace ECS{

    using UPComponentManager = std::unique_ptr<ComponentManager>;
    class ComponentManager {

        std::map<unsigned int, UPComponent>componentsMap;
        ComponentManager(){

        }

    public:

        static UPComponentManager cm;
        UPComponentManager GetComponentManager(){

            if (cm == nullptr){
                cm = std::make_unique(new ComponentManager());
            }
            return cm;

        }

        template <typename C>
        unsigned int CreateComponent(){

            auto pcompo = std::make_unique<C>(new C());
            componentsMap[pcompo->m_id] = pcompo;
            return pcompo->m_id;

        }

        bool AddComponent(UPEntity pent, UPComponent pcompo){

            pent->m_components.push_back(pcompo);
            return true;
        }

        UPComponent GetComponent(unsigned int compoId){
            return componentsMap[compoId];
        }


    };
    UPComponentManager ComponentManager::cm = nullptr;

}


#endif /* __COMPONENT_MANAGER_H__ */