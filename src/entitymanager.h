#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "entity.h"


namespace ECS {

    class EntityManager{

        EntityManger(){}
        static UPEntityManager em;

    public:

        static UPEntityManager GetManager(){
            if (em == nullptr){
                em = std::make_unique(new EntityManager());
            }
            return em;
        }
        template <typename T>
        unsigned int CreateEntity(){
            
        }
    };
}

#endif /* __ENTITYMANAGER_H__ */

