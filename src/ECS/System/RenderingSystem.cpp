

#include <Tech/G/Tech.h>
#include "RenderingSystem.h"

namespace ECS {

    unsigned int RenderingSystem::SubscribeEntity(unsigned int id){

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

        textures.push_back(pTexture.get());                 textures_   = textures.data();
        anchors.push_back(pSpriteComponent->GetAnchor());   anchors_    = anchors.data();
        scales.push_back(pSpriteComponent->GetScale());     scales_     = scales.data();
        positions.push_back(pPositionComponent);            positions_  = positions.data();
        ids.push_back(id);                                  ids_        = ids.data();

        return 1;

    }
    unsigned int RenderingSystem::DrawSprites(const GTech::Tech& rptech){

        auto sz = textures.size();
        for (auto index = 0; index < sz; ++index){

            auto pTexture = textures_[index];
            if (!pTexture) continue;

            auto rAnchor = anchors_[index];

            auto rScale = scales_[index];

            auto rPosition = positions_[index];

            GTech::Texture2DSize wsz;
            rptech->GetWindowSize(wsz);


            GTech::Rectangle2D dstrect;
            if (pTexture->GetSize(dstrect.winSz) == GTECH_ERROR) continue;

            dstrect.winSz.w *= *rScale;
            dstrect.winSz.h *= *rScale;

            GTech::Point2D anchorPoint;
            anchorPoint.x = (dstrect.winSz.w - 1) * rAnchor->x;
            anchorPoint.y = (dstrect.winSz.h - 1) - (dstrect.winSz.h - 1) * rAnchor->y;

            dstrect.winPos.x = rPosition->x - anchorPoint.x;
            dstrect.winPos.y = rPosition->y - anchorPoint.y;

            rptech->RenderTextureEx(pTexture, dstrect, GTech::Zero, 0.0, anchorPoint, GTech::FlipType::FLIP_NO);

        }

        return 1;
    }
    int RenderingSystem::InitRenderingSystem(const GTech::Tech& rpTech) {

        Texture2DSize sz;
        rpTech->GetWindowSize(sz);

        pScreen.reset();
        pScreen = rpTech->CreateTextureWithSize(sz);

        rpTech->SetRenderTarget(pScreen);
        rpTech->RenderClear();

        return pScreen ? true : false;

    }
    bool RenderingSystem::ShutdownRenderingSystem(const GTech::Tech& rpTech) {

        rpTech->DestroyTexture(pScreen);
        return rpTech->DestroyTexture(pScreen) == GTECH_OK ? true : false;

    }
    void RenderingSystem::UpdateRenderingSystem(const GTech::Tech& rpTech){

        rpTech->DetachRenderTexture();
        rpTech->RenderClear();
        rpTech->RenderTextureEx(pScreen, GTech::Zero, GTech::Zero, 0, pScreen->Center(), GTech::FlipType::FLIP_NO);
        rpTech->UpdateScreen();
        rpTech->SetRenderTarget(pScreen);

    }
    std::vector<unsigned int>               RenderingSystem::ids{};
    std::vector<GTech::Texture2D*>          RenderingSystem::textures{};
    std::vector<GTech::Vector2Dd*>          RenderingSystem::anchors{};
    std::vector<double*>                    RenderingSystem::scales{};
    std::vector<ECS::PositionComponent*>    RenderingSystem::positions{};
    unsigned int*                           RenderingSystem::ids_        = nullptr;
    GTech::Texture2D**                      RenderingSystem::textures_   = nullptr;
    GTech::Vector2Dd**                      RenderingSystem::anchors_    = nullptr;
    double**                                RenderingSystem::scales_     = nullptr;
    ECS::PositionComponent**                RenderingSystem::positions_  = nullptr;
    GTech::Texture                          RenderingSystem::pScreen     = nullptr;

}