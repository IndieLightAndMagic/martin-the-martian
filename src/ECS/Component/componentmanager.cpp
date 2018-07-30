#include "componentmanager.h"

using namespace ECS;

ComponentManager& ComponentManager::GetInstance(){

    static ComponentManager cm;
    return cm;

}

Component ComponentManager::GetComponent(unsigned int componentId) const {

    return componentMap[componentId];

}

const EntityInformationComponent_& ComponentManager::GetInformationComponent(unsigned int entityId) {

    static auto&	componentManager    = ECS::ComponentManager::GetInstance();
    auto 			entityInfoId		= ECS::EntityManager::GetInstance().GetComponentsIds(entityId)[0];
    auto  			infoComponentRP		= componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityInfoId);

    return infoComponentRP[0];

}

