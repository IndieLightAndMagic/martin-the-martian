
#include <ECS/System/rendering.h>

namespace ECS {


    class ECSUtil{
        static ECS::RenderingDataTuple GetRenderingTuple(unsigned int identityId){

            static auto&    componentManager        = ECS::ComponentManager::GetInstance();
            auto            entityInfoComponentId   = ECS::EntityManager::GetInstance().GetComponentsIds(identityId)[0];
            auto            infoComponentRP         = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityInfoComponentId);


        }
    };

	
}