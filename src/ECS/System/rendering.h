#ifndef __RENDERING__ 
#define __RENDERING__ 

#include <set>
#include <vector>
#include <utility>

#include <Tech/G/Tech.h>
#include <SDL2/SDL_system.h>
#include <ECS/Component/component.h>
#include <ECS/Component/componentmanager.h>
#include <ECS/Entity/entitymanager.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

SDL_Texture* SDLCreateTexture(SDL_Rect& rSize);
void SDLDetachRenderTexture();
void SDLRenderClear();
void SDLRenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
void SDLSetRenderTarget(SDL_Texture* pSDLTexture);
void SDLUpdateScreen();
std::pair<int, int> SDLWindowSize();

namespace ECS {
    class RenderingSystem {


        static std::vector<unsigned int>    ids;
        static std::vector<SDL_Texture*>    textures;
        static std::vector<glm::ivec2>      textureSizes;
        static std::vector<glm::vec3*>      positions;


        //Screen Context
        static SDL_Texture* pScreen;
        
    public:
        static unsigned int SubscribeEntity(unsigned int id){

            //Get Manager
            auto entityManager = ECS::EntityManager_::GetManager();
            auto componentManager = ECS::ComponentManager_::GetManager();
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

                auto rawPtr = pComponent.get();
                if (pComponentType == "ECS::PositionComponent")     pPositionComponent = dynamic_cast<PositionComponent*>(pComponent.get());
                else if (pComponentType == "ECS::SpriteComponent")  pSpriteComponent = dynamic_cast<SpriteComponent*>(pComponent.get());

            }

            if (compotypes.size() > 0 || pSpriteComponent == nullptr || pPositionComponent == nullptr){

                for (auto &type: compotypes){
                    std::cout << "RenderingSystem::SubscribeEntity : " << "Component not found: " << type << "\n";
                }
                return 0;

            }

            auto pTexture = pSpriteComponent->GetTexture();


            //Texture Data
            textures.push_back(pTexture);
            int w,h;
            SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);

            auto sizevec = glm::ivec2(w,h);
            textureSizes.push_back(sizevec);

            //Position data
            positions.push_back(&pPositionComponent->position);

            ids.push_back(id);

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

                //Render pTexture,
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
 
