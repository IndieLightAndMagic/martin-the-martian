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

        //Add Components to spriteId
        auto accelerationComponentId = componentManager.CreateComponent<ECS::AccelerationComponent_>();
        auto positionComponentId     = componentManager.CreateComponent<ECS::PositionComponent_>();
        auto speedComponentId        = componentManager.CreateComponent<ECS::SpeedComponent_>();

        entityManager.AddComponent(spriteId, accelerationComponentId);
        entityManager.AddComponent(spriteId, positionComponentId);
        entityManager.AddComponent(spriteId, speedComponentId);
        entityManager.AddComponent(spriteId, textureComponentId);

        //Subscribe to kinematic elements
        auto informationId  = entityManager.GetComponentsIds(spriteId)[0];
        auto informationRP  = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(informationId);
        informationRP->SetKinematicTupleIds(positionComponentId, speedComponentId, accelerationComponentId);
        informationRP->SetRenderingTupleIds(positionComponentId, textureComponentId);

        return spriteId;

    }

    void Sprite::SetPosition(unsigned int shipId, glm::vec3 position) {

        auto&	componentManager = ECS::ComponentManager::GetInstance();
        auto 	entityInfoId = ECS::EntityManager::GetInstance().GetComponentsIds(shipId)[0];		
        auto  	infoComponentRP = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityInfoId);
        auto  	[positionId, textureId] = infoComponentRP->GetRenderingTupleIds();
        auto  	positionComponent = componentManager.GetComponentRaw<ECS::PositionComponent_>(positionId);
        
        positionComponent->position = position;

    }

    void Sprite::SetAnchor(unsigned int shipId, glm::vec3 anchor) {

        auto& componentInfo             = ECS::ComponentManager::GetInformationComponent(shipId);
        auto  [positionId, textureId]   = componentInfo.GetRenderingTupleIds();
        auto  positionComponent         = ECS::ComponentManager::GetInstance().GetComponentRaw<ECS::PositionComponent_>(shipId);

        positionComponent->anchor = anchor;

    }

}