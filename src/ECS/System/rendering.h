#ifndef __RENDERING__ 
#define __RENDERING__ 

#include <set>
#include <vector>
#include <utility>

#include <Tech/G/Tech.h>
#include <SDL2/SDL_system.h>
#include <ECS/Component/component.h>
#include <ECS/Component/componentmanager.h>
#include <entitymanager.h>



SDL_Texture* SDLCreateTexture(SDL_Rect& rSize);
void SDLDetachRenderTexture();
void SDLRenderClear();
void SDLRenderCopy(SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
void SDLSetRenderTarget(SDL_Texture* pSDLTexture);
void SDLUpdateScreen();
std::pair<int, int> SDLWindowSize();

namespace ECS {
    class RenderingSystem {


        static std::vector<unsigned int>            ids;
        static std::vector<SDL_Texture*>       		textures;
        static std::vector<ECS::PositionComponent*> positions;


        //Screen Context
        static SDL_Texture* pScreen;
        
    public:
        static unsigned int SubscribeEntity(unsigned int id){

            //Get Manager
            auto entityManager = ECS::EntityManager_::GetManager();
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

            textures.push_back(pTexture);
            positions.push_back(pPositionComponent);
            ids.push_back(id);                      

            return 1;

        }
        static unsigned int DrawSprites(const GTech::Tech& rptech){

            auto sz = textures.size();
            for (auto index = 0; index < sz; ++index){

                auto pTexture = textures[index];
                if (!pTexture) continue;

                auto rPosition = positions[index];

                
                auto wh = SDLWindowSize();


                //Render pTexture,
                SDL_Rect dstrect;
                dstrect.w = wh.first;
                dstrect.h = wh.second;
                dstrect.x = rPosition->x;
                dstrect.y = rPosition->y;

                SDLRenderCopy(pTexture, &dstrect, nullptr);
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
 
