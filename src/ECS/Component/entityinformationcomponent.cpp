

#include <ECS/Component/entityinformationcomponent.h>

using namespace std;
using namespace ECS;

vector<tuple<unsigned int, unsigned int, unsigned int>>  EntityInformationComponent_::GetKinematicTuples() const{
    return kinematicTupleIds;
};


void EntityInformationComponent_::SetKinematicTupleIds(unsigned int posId, unsigned int speedId, unsigned int accelId){
    kinematicTupleIds.push_back(tuple<unsigned  int, unsigned int, unsigned  int>{posId, speedId, accelId}) ;
}

tuple<unsigned int, unsigned int, unsigned int>  EntityInformationComponent_::GetRenderingTupleIds() const{
    return renderingTupleIds;
};

void EntityInformationComponent_::SetRenderingTupleIds(unsigned int posId, unsigned int anchorId, unsigned int textureId){
    renderingTupleIds = tuple<unsigned  int, unsigned int, unsigned int>{posId, anchorId, textureId};
}

