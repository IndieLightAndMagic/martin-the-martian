#ifndef __ENTITYINFORMATIONCOMPONENT_H__
#define __ENTITYINFORMATIONCOMPONENT_H__

#include <vector>
#include <tuple>
#include <ECS/Component/component.h>
#include <ECS/Component/componentmanager.h>

namespace ECS {

    class EntityInformationComponent_ : public Component_ {

        std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> m_kinematicTupleIds{};

        std::tuple<unsigned int, unsigned int, unsigned int, unsigned int> m_renderingTupleIds;

        public:
        EntityInformationComponent_() = default;

        /*!
         * @brief Returns a vector of component-id tuples of <positionComponentId, speedComponentId, accelComponentId>, which are the ids of the kinematic compound.
         *
         * @return A tuple with the component Ids of the kinematic triad compound.
         *
         * @code
         *
         * auto [posId, speedId, accelId] = informationComponent.kinematicTupleIds();
         * auto& compoManager = ECS::ComponentManager::GetInstance();
         *
         * auto pPositionComponent = compoManager.GetComponentRaw<ECS::PositionComponent_>(posId);
         * auto pSpeedComponentPtr = compoManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
         * auto pAccelComponent = compoManager.GetComponentRaw<ECS::AccelComponent_>(accelId);
         *
         * speedComponent->x = 45.0f;
         *
         * @endcode
         */
        std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> GetKinematicTuples() const;
        void SetKinematicTupleIds(unsigned int posId, unsigned int speedId, unsigned int accelId);

        /*!
         * @brief Returns a component id tuple of <positionComponentId, anglePositionId, anchorPointId, textureComponentId>, which are the ids of the rendering compound.
         *
         * @return A tuple with the component Ids of the rendering compound.
         *
         * @code
         *
         * auto [posId, anglePositionId, anchorPointId, textureId] = informationComponent.renderingTupleIds();
         * auto& compoManager = ECS::ComponentManager::GetInstance();
         *
         * auto pPositionComponent      = compoManager.GetComponentRaw<ECS::PositionComponent_>(posId);
         * auto pAngularPosComponent    = compoManager.GetComponentRaw<ECS::PositionComponent_>(anglePositionId);
         * auto pTextureComponent       = compoManager.GetComponentRaw<ECS::TextureComponent_>(textureId);
         * auto pAnchorPointComponent   = compoManager.GetComponentRaw<ECS::AnchorPointComponent_>(anchorPointId);
         *
         * auto [w, h] = pTextureComponent-> GetScaledSize();
         *
         * @endcode
         */
        std::tuple<unsigned int, unsigned int, unsigned int, unsigned int> GetRenderingTupleIds() const;
        void SetRenderingTupleIds(unsigned int posId, unsigned int anglePositionId, unsigned int anchorId, unsigned int textureComponentId);

    };

}


#endif //__ENTITYINFORMATIONCOMPONENT_H__
