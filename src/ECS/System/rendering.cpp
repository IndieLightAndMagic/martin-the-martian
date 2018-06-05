#include "rendering.h"

using namespace ECS;
std::vector<unsigned int>               RenderingSystem::ids{};
std::vector<SDL_Texture*>          		RenderingSystem::textures{};
std::vector<glm::ivec2>                 RenderingSystem::textureSizes{};
std::vector<glm::vec3*>	                RenderingSystem::positions{};
SDL_Texture* 							RenderingSystem::pScreen = nullptr;
SDL_Rect                                RenderingSystem::pScreenRect{0, 0, 0, 0};
glm::mat4x4                             RenderingSystem::mtxSDLScreenCoordinates({1.0f, 0.0f, 0.0f, 0},{0.0f, 1.0f, 0.0f, 0.0f},{0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0});
std::vector<
        std::tuple<
                const unsigned int,   ///ids
                SDL_Texture*,         ///textures
                const glm::ivec2,     ///textureSizes
                glm::vec3*,           ///positions
                bool&                 ///dirty
        >>                              RenderingSystem::renderingData{};


namespace ECS {
    unsigned long RenderingSystem::SubscribeEntity(unsigned int entityId) {

            //Get Managers
            auto& entityManager     = ECS::EntityManager::GetInstance();
            auto& componentManager  = ECS::ComponentManager::GetInstance();

            auto& entityComponentsV     = entityManager.GetComponentsIds(entityId);
            auto& entityComponentInfoId = entityComponentsV[0];

            //Info Component
            auto entityComponentInfoRP  = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityComponentInfoId);
            auto [posId, textureId]     = entityComponentInfoRP->GetRenderingTupleIds();


            auto pSpriteComponent   = componentManager.GetComponentRaw<ECS::TextureComponent_>(textureId);
            auto pPositionComponent = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId);
            pPositionComponent->isDirty = true;

            //Texture Data
            auto pTexture = pSpriteComponent->GetTexture();
            textures.push_back(pTexture);

            auto [format, access, sz] = SDLQueryTexture(pTexture);
            textureSizes.push_back(sz);

            //Position data
            positions.push_back(&pPositionComponent->position);

            //Entity Id Data
            ids.push_back(entityId);

            auto entityRenderingData = std::tuple<
                    const unsigned int, ///ids
                    SDL_Texture*,       ///textures
                    const glm::ivec2,   ///textureSizes
                    glm::vec3*,         ///positions
                    bool&               ///Check If Dirty
            >(entityId, pTexture, sz, &pPositionComponent->position, pPositionComponent->isDirty);
            renderingData.emplace_back(entityRenderingData);

            unsigned long size = renderingData.size();
            SDL_assert(size>0);
            return size-1;

    }
    unsigned int RenderingSystem::DrawSprites() {

        auto sz = textures.size();
        for (auto& [eid, pTexture, textureSize, pPosition, dirty]: renderingData){

            if (!dirty || !pTexture) continue;

            //Render m_pTexture,
            SDL_Rect dstrect;

            dstrect.x = pPosition->x;
            dstrect.y = pPosition->y;
            dstrect.w = textureSize.x;
            dstrect.h = textureSize.y;
            SDL_QueryTexture(pTexture, nullptr, nullptr, &dstrect.w, &dstrect.h);
            SDLRenderCopy(pTexture, nullptr, &dstrect);
            dirty = false;
        }

        return 1;
    }
    void RenderingSystem::InitRenderingSystem() {

        auto wh = SDLWindowSize();

        pScreen = nullptr;

        pScreenRect.w = wh.first;
        pScreenRect.h = wh.second;
        pScreen = SDLCreateTexture(pScreenRect);

        SDLSetRenderTarget(pScreen);
        SDLRenderClear();

    }
    void RenderingSystem::ShutdownRenderingSystem() {

        SDL_DestroyTexture(pScreen);
        pScreen = nullptr;

    }
    void RenderingSystem::UpdateRenderingSystem() {

        SDLDetachRenderTexture();
        SDLRenderClear();
        SDLRenderCopy(pScreen, nullptr, nullptr);
        SDLUpdateScreen();
        SDLSetRenderTarget(pScreen);

    }
}