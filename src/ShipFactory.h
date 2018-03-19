#ifndef __SHIPSPRITEFACTORY_H__
#define __SHIPSPRITEFACTORY_H__

#include <set>

#include "Tech.h"
#include "entitymanager.h"
#include "componentmanager.h"

namespace GAME {



    class ShipFactory {

    public:
        static unsigned int CreateShip(const GTech2D::GTPTech2D& rptech){

            auto entityManager = ECS::EntityManager::GetManager();
            auto componentManager = ECS::ComponentManager::GetManager();

            //Create Ship Entity
            auto shipId = entityManager->CreateEntity();

            //Create a Position & Speed Components
            auto positionComponent = componentManager->CreateComponent<ECS::PositionComponent>();
            auto speedComponent = componentManager->CreateComponent<ECS::SpeedComponent>();

            auto shipTexture = rptech->LoadTexture("hero.png");

            auto spriteComponent = componentManager->CreateComponent<ECS::SpriteComponent>();
            auto spSpriteComponent = componentManager->GetComponent(spriteComponent);
            auto rpSpriteComponent = static_cast<ECS::SpriteComponent*>(spSpriteComponent.get());

            rpSpriteComponent->SetTexture(shipTexture);
            rpSpriteComponent->SetAnchor({0.5,0.5});


            //Add Components to ship
            entityManager->AddComponent(shipId, positionComponent);
            entityManager->AddComponent(shipId, speedComponent);
            entityManager->AddComponent(shipId, spriteComponent);


            return shipId;
        }
        static void SetShipPosition(unsigned int shipId, int x, int y){

            auto componentsList = ECS::EntityManager::GetManager()->GetComponentIds(shipId);
            auto genericComponent = ECS::ComponentManager::GetManager()->GetComponent(componentsList[0]);
            auto rawPointerGenericComponent = genericComponent.get();
            auto pointerPositionComponent = dynamic_cast<ECS::PositionComponent*>(rawPointerGenericComponent);
            pointerPositionComponent->x = x;
            pointerPositionComponent->y = y;
            pointerPositionComponent->z = 0;

        }
    };

    class RenderingSystem {

        static std::vector<GTech2D::Texture2D*>  textures;
        static std::vector<GTech2D::Vector2Dd*>     anchors;
        static std::vector<double*>                 scales;
        static std::vector<ECS::PositionComponent*> positions;

    public:
        static unsigned int SubscribeEntity(unsigned int id){

            //Get Manager
            auto entityManager = ECS::EntityManager::GetManager();
            auto componentManager = ECS::ComponentManager::GetManager();
            auto componentVectors = entityManager->GetComponentIds(id);

            std::set<std::string> compotypes{"ECS::PositionComponent","ECS::SpriteComponent"};

            ECS::SpriteComponent* pSpriteComponent = nullptr;
            ECS::PositionComponent* pPositionComponent = nullptr;

            for (auto& componentId:componentVectors){

                auto pComponent = componentManager->GetComponent(componentId);
                auto pComponentType = pComponent->GetType();

                //Search
                auto pit = compotypes.find(pComponentType);
                if (pit == compotypes.end()) continue;

                //Found & erase
                compotypes.erase(pit);

                //Hook to pointers
                ECS::ECSPComponent_ wpComponent = pComponent;
                if (auto spComponent = wpComponent.lock()){

                    auto rawPointer = spComponent.get();
                    if (pComponentType == "ECS::SpriteComponent")           pSpriteComponent = static_cast<ECS::SpriteComponent*>(rawPointer);
                    else if (pComponentType == "ECS::PositionComponent")    pPositionComponent = static_cast<ECS::PositionComponent*>(rawPointer);

                }

            }

            if (compotypes.size() > 0 || pSpriteComponent == nullptr || pPositionComponent == nullptr){

                for (auto &type: compotypes){
                    std::cout << "RenderingSystem::SubscribeEntity : " << "Component not found: " << type << "\n";
                }
                if (!pSpriteComponent) std::cout << "RenderingSystem::SubscribeEntity : pSpriteComponent is null\n";
                if (!pPositionComponent) std::cout << "RenderingSystem::SubscribeEntity : pSpriteComponent is null\n";

                return 0;

            }
            auto pTexture = pSpriteComponent->GetTexture();
            textures.push_back(pTexture.get());

            anchors.push_back(pSpriteComponent->GetAnchor());
            scales.push_back(pSpriteComponent->GetScale());
            positions.push_back(pPositionComponent);

            return 1;

        }
        static unsigned int DrawSprites(const GTech2D::GTPTech2D& rptech){

            auto sz = textures.size();
            for (auto index = 0; index < sz; ++index){

                auto pTexture = textures[index];
                if (!pTexture) continue;

                auto rAnchor = anchors[index];

                auto rScale = scales[index];

                auto rPosition = positions[index];

                GTech2D::Texture2DSize wsz;
                rptech->GetWindowSize(wsz);


                GTech2D::Rectangle2D dstrect;
                if (pTexture->GetSize(dstrect.winSz) == GTECH_ERROR) continue;

                dstrect.winSz.w *= *rScale;
                dstrect.winSz.h *= *rScale;

                GTech2D::Point2D anchorPoint;
                anchorPoint.x = (dstrect.winSz.w - 1) * rAnchor->x;
                anchorPoint.y = (dstrect.winSz.h - 1) - (dstrect.winSz.h - 1) * rAnchor->y;

                dstrect.winPos.x = rPosition->x - anchorPoint.x;
                dstrect.winPos.y = rPosition->y - anchorPoint.y;

                rptech->RenderTextureEx(pTexture, dstrect, GTech2D::Zero, 0.0, anchorPoint, GTech2D::FlipType::FLIP_NO);

                SDL_Log("Hi!");

            }

            return 1;
        }

        static void Update(const GTech2D::GTPTech2D& rpTech){
            DrawSprites(rpTech);
        }
    };
    std::vector<GTech2D::Texture2D*>        RenderingSystem::textures{};
    std::vector<GTech2D::Vector2Dd*>        RenderingSystem::anchors{};
    std::vector<double*>                    RenderingSystem::scales{};
    std::vector<ECS::PositionComponent*>    RenderingSystem::positions{};

}


#endif /* __SHIPSPRITEFACTORY_H__ */