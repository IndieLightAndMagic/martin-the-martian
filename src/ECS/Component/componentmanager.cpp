#include "componentmanager.h"

using namespace ECS;

ComponentManager& ComponentManager::GetInstance(){

    static ComponentManager cm;
    return cm;

}

Component ComponentManager::GetComponent(unsigned int componentId) {

    return componentMap[componentId];

}

