#include <ECS/Component/componentmanager.h>
#include <ECS/Component/entityinformationcomponent.h>
#include "ECS/Entity/entitymanager.h"

using namespace std;
using namespace ECS;


EntityManager& EntityManager::GetInstance(){

    static EntityManager em;
    return em;

}
unsigned int EntityManager::CreateEntity(){

    Entity pEntity = EntityFactory::CreateEntity();
    entityMap[pEntity->m_id] = pEntity;

    //InformationId
    auto& componentManager  = ComponentManager::GetInstance();
    auto informationId      = componentManager.CreateComponent<ECS::EntityInformationComponent_>();

    AddComponent(pEntity->m_id, informationId);

    return pEntity->m_id;

}

bool EntityManager::AddComponent(unsigned int entity, unsigned int componentId){

    entityMap[entity]->m_componentIds.push_back(componentId);
    return true;

}

const vector<unsigned int>& EntityManager::GetComponentsIds(unsigned int entity){

    return entityMap[entity]->m_componentIds;

}

Entity EntityManager::GetEntity(unsigned int entity){

    return entityMap[entity];

}