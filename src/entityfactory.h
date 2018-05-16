#ifndef __ENTITYFACTORY_H__
#define __ENTITYFACTORY_H__
#include "entity.h"


namespace ECS {
    class EntityFactory {

    public:
        static unsigned int m_int;

        static ECSPEntity CreateEntity(){

            ECSPEntity pEnt = std::make_shared<Entity>();
            pEnt->m_id = ++m_int;
            return pEnt;

        }

    };
    

}
#endif //__ENTITYFACTORY_H__
