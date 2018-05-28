#include "componentmanager.h"

using namespace ECS;
ComponentManager_::ComponentManager ComponentManager_::componentManager = nullptr;


ComponentManager_::ComponentManager ComponentManager_::GetManager(){

    if ( componentManager == nullptr ){
        componentManager = std::make_shared<ComponentManager_>();
    }
    return componentManager;
}

Component ComponentManager_::GetComponent(unsigned int componentId){
	return componentMap[componentId];
}
