#ifndef __RENDERING__ 
#define __RENDERING__ 

#include <set>
#include <vector>
#include <utility>

#include <SDL2/SDL_system.h>

#include <sdlwrapper.h>
#include <ECS/Component/component.h>
#include <ECS/Component/componentmanager.h>
#include <ECS/Entity/entitymanager.h>
#include <ECS/Component/entityinformationcomponent.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>


namespace ECS {
    class RenderingSystem {


        static std::vector<unsigned int>    ids;
        static std::vector<SDL_Texture*>    textures;
        static std::vector<glm::ivec2>      textureSizes;
        static std::vector<glm::vec3*>      positions;


        //Screen Context
        static SDL_Texture* pScreen;
        
    public:
        static unsigned int SubscribeEntity(unsigned int entityId){

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

            //Texture Data
            auto pTexture = pSpriteComponent->GetTexture();
            textures.push_back(pTexture);

            auto [format, access, sz] = SDLQueryTexture(pTexture);
            textureSizes.push_back(sz);

            //Position data
            positions.push_back(&pPositionComponent->position);

            ids.push_back(entityId);

            return 1;

        }
        static unsigned int DrawSprites(){

            auto sz = textures.size();
            for (auto index = 0; index < sz; ++index){

                //Got texture & texture sizes
                auto pTexture = textures[index];
                if (!pTexture) continue;


                auto textureSize = textureSizes[index];

                //Got Position
                auto rPosition = positions[index];

                //Render m_pTexture,
                SDL_Rect dstrect;

                dstrect.x = rPosition->x;
                dstrect.y = rPosition->y;
                dstrect.w = textureSize.x;
                dstrect.h = textureSize.y;
                SDL_QueryTexture(pTexture, nullptr, nullptr, &dstrect.w, &dstrect.h);
                SDLRenderCopy(pTexture, nullptr, &dstrect);
            }

            return 1;
        }
        static void InitRenderingSystem() {
            
            auto wh = SDLWindowSize();

            pScreen = nullptr;
            SDL_Rect rect;
            rect.w = wh.first;
            rect.h = wh.second;
            pScreen = SDLCreateTexture(rect);

            SDLSetRenderTarget(pScreen);
            SDLRenderClear();


        }
        static void ShutdownRenderingSystem() {

            SDL_DestroyTexture(pScreen);
            pScreen = nullptr;

        }

        static void UpdateRenderingSystem(){

            SDLDetachRenderTexture();
            SDLRenderClear();
            SDLRenderCopy(pScreen, nullptr, nullptr);
            SDLUpdateScreen();
            SDLSetRenderTarget(pScreen);

        }
    };

}
#endif /*__RENDERING__ */
 
