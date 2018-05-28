#ifndef __COMPONENTFACTORY_H__
#define __COMPONENTFACTORY_H__

#include "component.h"
namespace ECS {

    class ComponentFactory {
    public:
        static unsigned int m_int;
        template <typename T>
        static Component CreateComponent(){
            Component pComp = std::make_shared<T>();
            pComp->m_id = ++m_int;
            return pComp;
        }
        template <typename T> static Component CreateComponent(T* pComponent){

            Component pComp = std::make_shared<T>();
            auto rawComponent = static_cast<ECS::Component_*>(pComponent);
            pComp.reset(rawComponent);
            pComp->m_id = ++m_int;
            return pComp;

        }
    };
    
}

#endif //__COMPONENTFACTORY_H__
