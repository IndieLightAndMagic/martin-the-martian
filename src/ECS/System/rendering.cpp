#include<ctime>
#include <tuple>
#include "rendering.h"

using namespace ECS;
SDL_Texture* 							RenderingSystem::pScreen = nullptr;
SDL_Rect                                RenderingSystem::pScreenRect{0, 0, 0, 0};
glm::mat4x4                             RenderingSystem::mtxSDLScreenCoordinates(
    {1.0f, 0.0f, 0.0f, 0},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0});

std::vector<RenderingDataTuple> RenderingSystem::renderingData{};

namespace ECS { //metodos no se pueden acceder solo por el nombre
    unsigned long RenderingSystem::SubscribeEntity(unsigned int entityId, int typeEntity) {

            //Get Managers
            auto& entityManager     = ECS::EntityManager::GetInstance();
            auto& componentManager  = ECS::ComponentManager::GetInstance();

            auto& entityComponentsV     = entityManager.GetComponentsIds(entityId);
            auto& entityComponentInfoId = entityComponentsV[0];

            //Info Component
            auto entityComponentInfoRP                              = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityComponentInfoId);
            auto [posId, anglePositionId, anchorId, textureId]      = entityComponentInfoRP->GetRenderingTupleIds();


            auto pTextureComponent          = componentManager.GetComponentRaw<ECS::TextureComponent_>(textureId);
            auto pPositionComponent         = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId);
            auto pAnglePositionComponent    = componentManager.GetComponentRaw<ECS::PositionComponent_>(anglePositionId);
            auto pAnchorPointComponent	    = componentManager.GetComponentRaw<ECS::AnchorPointComponent_>(anchorId);

            pPositionComponent->isDirty = true;

            //Texture Data
            auto pTexture = pTextureComponent->GetTexture();

            auto sz =  &pTextureComponent->m_scaledSize_16W_16H;
            
            std::time_t t = std::time(0);   // get time now
            long timeCreation = (long)t;

            RenderingDataTuple entityRenderingData(//
                    entityId, //identifcacion de componente
                    pTexture,
                    sz,
                    &pPositionComponent->position,
                    &pAnglePositionComponent->position,
                    &pAnchorPointComponent->m_anchorPoint,
                    &pAnchorPointComponent->m_correctionVector,
                    &pPositionComponent->isDirty,
                    typeEntity,
                    timeCreation);

            renderingData.emplace_back(entityRenderingData);

            unsigned long size = renderingData.size();
            SDL_assert(size>0);

            std::sort(begin(renderingData), end(renderingData), [](auto t1, auto t2){
                return reinterpret_cast<glm::vec3*>(std::get<3>(t1))->z < reinterpret_cast<glm::vec3*>(std::get<3>(t2))->z;
            });
            return size-1;

    }
    unsigned int RenderingSystem::DrawSprites2D() {

        SDLRenderClear();

        for (auto& [eid, pTexture, pEncodedTextureSize, pvPosition, pvAnglePosition, pvAnchorPoint, pvAnchorPointCorrection, pDirty, pTypeEntity, pTimeCreation]: renderingData){

            auto encodedTextureSize         = *pEncodedTextureSize;
            auto pPosition                  = reinterpret_cast<glm::vec3*>  (pvPosition);
            auto pAnglePosition             = reinterpret_cast<glm::vec3*>  (pvAnglePosition);
            auto pAnchorPoint               = reinterpret_cast<glm::vec3*>  (pvAnchorPoint);
            auto pAnchorPointCorrection     = reinterpret_cast<glm::vec3*>  (pvAnchorPointCorrection);

            //Check if need to render
            //if (!pTexture) continue;

            //Render m_pTexture,
            SDL_Rect dstrect;

            //AABB
            dstrect.x = pPosition->x + pAnchorPointCorrection->x;
            dstrect.y = pPosition->y + pAnchorPointCorrection->y;
            dstrect.w = encodedTextureSize >> 16;
            dstrect.h = encodedTextureSize & 0xffff;


            SDLRenderCopyEx(pTexture, nullptr, &dstrect, pAnglePosition->z, pAnchorPoint);
            //SDLRenderCopy(pTexture, nullptr, &dstrect);
            *pDirty = false;

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

        SDLSetRenderTarget(pScreen);                //SELECT PSCREEN AS THE ACTIVE CANVAS
        SDLRenderClear();                           //TAKE THE PSCREEN TEXTURE AKA ACTUAL CANVAS AND CLEAR IT
        DrawSprites2D();                              //DRAW THE SHIPS AND THE FIRE AND THE STUFF IN THE PSCREEN CANVAS
        SDLDetachRenderTexture();                   //NOW IS THE GPU WHERE THE RENDERER WILL DRAW
        SDLRenderCopy(pScreen, nullptr, nullptr);   //TAKE PSCREEN AND COPY IT INTO THE GPU
        SDLUpdateScreen();                          //UPDATE THE SCREEN
        DesaparecerBalas(5);                        //llama la metodo
    }

    void RenderingSystem::DesaparecerBalas(int sec){
        
        RenderingDataTuple Item;

        for (std::vector<RenderingDataTuple>::iterator it = renderingData.begin() ; it != renderingData.end(); ++it) {
            Item = *it;
            long time = (long)std::get<9>(Item);
            int typeEntity= (int)std::get<8>(Item); 
             
            if(typeEntity == 2){
                std::time_t t = std::time(0);   // get time now
                long end = (long)t;  

                if((end-time) >= sec){
                    renderingData.erase(it);
                    
                }
            }       
        }
    }

}

