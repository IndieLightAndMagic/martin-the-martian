#include "componentmanager.h"

using namespace ECS;
ComponentManager::ECSPComponentManager ComponentManager::componentManager = nullptr;


ComponentManager::ECSPComponentManager ComponentManager::GetManager(){

    if ( componentManager == nullptr ){
        componentManager = std::make_shared<ComponentManager>();
    }
    return componentManager;
}

ECSPComponent ComponentManager::GetComponent(unsigned int componentId){
	return componentMap[componentId];
}
