

#include <ECS/Component/entityinformationcomponent.h>

using namespace std;
using namespace ECS;

vector<tuple<unsigned int, unsigned int, unsigned int>>  EntityInformationComponent_::GetKinematicTuples() const{
    return m_kinematicTupleIds;
};


void EntityInformationComponent_::SetKinematicTupleIds(unsigned int posId, unsigned int speedId, unsigned int accelId){
    m_kinematicTupleIds.push_back(tuple<unsigned  int, unsigned int, unsigned  int>{posId, speedId, accelId}) ;
}

tuple<unsigned int, unsigned int, unsigned int, unsigned int> EntityInformationComponent_::GetRenderingTupleIds() const{
    return m_renderingTupleIds;
};

void EntityInformationComponent_::SetRenderingTupleIds(unsigned int posId, unsigned int anglePositionId, unsigned int anchorId, unsigned int textureId) {
    m_renderingTupleIds = tuple<unsigned  int, unsigned int, unsigned int, unsigned int>{posId, anglePositionId, anchorId, textureId};
}

