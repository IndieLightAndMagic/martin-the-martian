

#include <ECS/Component/entityinformationcomponent.h>

using namespace std;
using namespace ECS;

std::tuple<unsigned int&, unsigned int&, unsigned int&>  EntityInformationComponent_::GetKinematicTupleIds(){
    return kinematicTupleIds;
};


void EntityInformationComponent_::SetKinematicTupleIds(unsigned int posId, unsigned int speedId, unsigned int accelId){
    kinematicTupleIds = std::tuple<unsigned  int, unsigned int, unsigned  int>{posId, speedId, accelId};
}

tuple<unsigned int&, unsigned int&>  EntityInformationComponent_::GetRenderingTupleIds(){
    return renderingTupleIds;
};


void EntityInformationComponent_::SetRenderingTupleIds(unsigned int posId, unsigned int textureId){
    renderingTupleIds = std::tuple<unsigned  int, unsigned int>{posId, textureId};
}

const EntityInformationComponent_& EntityInformationComponent_::GetInformationComponent(unsigned int entityId) {

    static auto&	ComponentManager 	= ECS::ComponentManager::GetInstance();
    auto 			entityInfoId		= ECS::EntityManager::GetInstance().GetComponentsIds(shipId)[0];		
    auto  			infoComponentRP		= componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityInfoId);


}