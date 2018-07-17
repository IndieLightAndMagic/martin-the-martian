#include <Sprite.h>
#include <ECS/Component/entityinformationcomponent.h>

using namespace std;

namespace GTech {

    unsigned int Sprite::CreateSprite(std::string path){

        auto& entityManager = ECS::EntityManager::GetInstance();
        auto& componentManager = ECS::ComponentManager::GetInstance();

        //Create Sprite Entity_a
        auto spriteId = entityManager.CreateEntity();

        //Create Texture Id
        auto textureComponentId = componentManager.CreateComponent<ECS::TextureComponent_>();
        auto textureComponentRP = componentManager.GetComponentRaw<ECS::TextureComponent_>(textureComponentId);

        //SetTexture
        textureComponentRP->SetTexture(path);
        auto [scaledTextureWidth, scaledTextureHeight] =  textureComponentRP->GetScaledSize();

        //Add Components to spriteId

        //Kinematic Cartesian Positions
        auto accelerationComponentId = componentManager.CreateComponent<ECS::AccelerationComponent_>();
        auto positionComponentId     = componentManager.CreateComponent<ECS::PositionComponent_>();
        auto speedComponentId        = componentManager.CreateComponent<ECS::SpeedComponent_>();

        //Kinematic Angular Positions
        auto accelerationAngularComponentId = componentManager.CreateComponent<ECS::AccelerationComponent_>();
        auto positionAngularComponentId     = componentManager.CreateComponent<ECS::PositionComponent_>();
        auto speedAngularComponentId        = componentManager.CreateComponent<ECS::SpeedComponent_>();

        //Create an anchor component
        auto anchorPointId          = componentManager.CreateComponent<ECS::AnchorPointComponent_>();

        entityManager.AddComponent(spriteId, accelerationComponentId);
        entityManager.AddComponent(spriteId, positionComponentId);
        entityManager.AddComponent(spriteId, speedComponentId);
        entityManager.AddComponent(spriteId, accelerationAngularComponentId);
        entityManager.AddComponent(spriteId, positionAngularComponentId);
        entityManager.AddComponent(spriteId, speedAngularComponentId);
        entityManager.AddComponent(spriteId, anchorPointId);
        entityManager.AddComponent(spriteId, textureComponentId);

        //Subscribe to kinematic elements
        auto informationId  = entityManager.GetComponentsIds(spriteId)[0];
        auto informationRP  = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(informationId);

        //Set Position Kinematics tuple.
        informationRP->SetKinematicTupleIds(positionComponentId, speedComponentId, accelerationComponentId);

        //Set Angular Kinematics tuple.
        informationRP->SetKinematicTupleIds(positionAngularComponentId, speedAngularComponentId, accelerationAngularComponentId);
        auto positionAngularComponent = componentManager.GetComponentRaw<ECS::PositionComponent_>(positionAngularComponentId);

        //Set Rendering Tuple
        informationRP->SetRenderingTupleIds(positionComponentId, positionAngularComponentId, anchorPointId, textureComponentId);

        //Set AnchorPoint
        Sprite::SetAnchorPoint(spriteId, glm::vec3{0.5f, 0.5f, 0.0f});
        
        //The ship is set return it.
        return spriteId;

    }

    void Sprite::SetPosition(unsigned int shipId, glm::vec3 position) {

        auto&	componentManager = ECS::ComponentManager::GetInstance();
        auto 	entityInfoId = ECS::EntityManager::GetInstance().GetComponentsIds(shipId)[0];		
        auto  	infoComponentRP = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityInfoId);
        auto  	[positionId, anglePositionId, anchorId, textureId] = infoComponentRP->GetRenderingTupleIds();
        auto  	positionComponent = componentManager.GetComponentRaw<ECS::PositionComponent_>(positionId);
        
        positionComponent->position = position;

    }

    void Sprite::SetAnchorPoint(unsigned int shipId, glm::vec3 anchor) {

        auto  componentManager                                      = ECS::ComponentManager::GetInstance();
        auto& componentInfo                                         = ECS::ComponentManager::GetInformationComponent(shipId);
        auto  [positionId, anglePositionId, anchorId, textureId]    = componentInfo.GetRenderingTupleIds();
        auto  anchorPointComponentRP                                = componentManager.GetComponentRaw<ECS::AnchorPointComponent_>(anchorId);
        auto  textureComponentRP                                    = componentManager.GetComponentRaw<ECS::TextureComponent_>(textureId);

        auto [scaledTextureWidth, scaledTextureHeight] = textureComponentRP->GetScaledSize();
        anchorPointComponentRP->SetAnchorPoint(anchor, glm::vec3{scaledTextureWidth, scaledTextureHeight, 0.0f});
    
    }
    void Sprite::SetScale(unsigned int shipId, float scale) {

        auto&	componentManager                                        = ECS::ComponentManager::GetInstance();
        auto 	entityInfoId                                            = ECS::EntityManager::GetInstance().GetComponentsIds(shipId)[0];
        auto  	infoComponentRP                                         = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityInfoId);
        auto  	[positionId, anglePositionId, anchorId, textureId]      = infoComponentRP->GetRenderingTupleIds();
        auto    textureComponentRP                                      = ECS::ComponentManager::GetInstance().GetComponentRaw<ECS::TextureComponent_>(textureId);
        auto    anchorPointComponentRP                                  = ECS::ComponentManager::GetInstance().GetComponentRaw<ECS::AnchorPointComponent_>(anchorId);

        textureComponentRP->SetScale(scale);
        auto [scaledTextureWidth, scaledTextureHeight] = textureComponentRP->GetScaledSize();
        anchorPointComponentRP->SetAnchorPoint(anchorPointComponentRP->m_anchorPoint, glm::vec3{scaledTextureWidth, scaledTextureHeight, 0.0f});


    }

}