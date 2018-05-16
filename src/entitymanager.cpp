#include "entitymanager.h"

using namespace ECS;

EntityManager_::ECSPEntityManager EntityManager_::entityManager = nullptr;



EntityManager_::ECSPEntityManager& EntityManager_::GetManager(){

	if ( entityManager == nullptr ){
		entityManager = std::make_unique<EntityManager_>();
	}
	return entityManager;

}

unsigned int EntityManager_::CreateEntity(){
    ECSPEntity pEntity = EntityFactory::CreateEntity();
    entityMap[pEntity->m_id] = pEntity;
    return pEntity->m_id;
}

bool EntityManager_::AddComponent(unsigned int entity, unsigned int componentId){
    entityMap[entity]->m_componentIds.push_back(componentId);
    return true;
}

std::vector<unsigned int>& EntityManager_::GetComponentIds(unsigned int entity){
    return entityMap[entity]->m_componentIds;
}

ECSPEntity EntityManager_::GetEntity(unsigned int entity){
    return entityMap[entity];
}