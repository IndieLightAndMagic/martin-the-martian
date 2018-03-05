#ifndef __COMPONENTFACTORY_H__
#define __COMPONENTFACTORY_H__

#include "component.h"
namespace ECS {

    class ComponentFactory {

    public:
        static unsigned int m_int;
        template <typename T>
        static ECSPComponent<T> CreateComponent(){

            ECSPComponent<T> pComp = std::make_shared< Component<T> >(new Component<T>());
            pComp->m_id = ++m_int;
            return pComp;

        }

    };
    unsigned int ComponentFactory::m_int = 0;
}

#endif //__COMPONENTFACTORY_H__
