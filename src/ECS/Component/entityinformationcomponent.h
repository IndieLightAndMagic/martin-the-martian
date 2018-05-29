#ifndef __ENTITYINFORMATIONCOMPONENT_H__
#define __ENTITYINFORMATIONCOMPONENT_H__

#include <tuple>
#include <ECS/Component/component.h>


namespace ECS {

    class EntityInformationComponent_ : public Component_ {
        std::tuple<unsigned int, unsigned int, unsigned int> kinematicTupleIds;
    public:
        EntityInformationComponent_() = default;
        std::tuple<unsigned int, unsigned int, unsigned int> GetKinematicTupleIds();
        void SetKinematicTupleIds(unsigned int posId, unsigned int speedId, unsigned int accelId);
    };

}


#endif //__ENTITYINFORMATIONCOMPONENT_H__
