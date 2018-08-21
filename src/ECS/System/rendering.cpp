#include <tuple>
#include "rendering.h"
#include <ctime>
#include <chrono>
#include <iomanip>

using namespace ECS;
SDL_Texture* 							RenderingSystem::pScreen = nullptr;
SDL_Rect                                RenderingSystem::pScreenRect{0, 0, 0, 0};
glm::mat4x4                             RenderingSystem::mtxSDLScreenCoordinates(
    {1.0f, 0.0f, 0.0f, 0},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0});


std::vector<RenderingDataTuple> RenderingSystem::renderingData{};

namespace ECS {
    unsigned long RenderingSystem::SubscribeEntity(unsigned int entityId, unsigned int typeComponent) {

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

            pPositionComponent->isDirty = false;

			//timer
			auto lifeTimeStart = std::chrono::steady_clock::now();

            //Texture Data
            auto pTexture = pTextureComponent->GetTexture();

            auto sz =  &pTextureComponent->m_scaledSize_16W_16H;

            RenderingDataTuple entityRenderingData(
                    entityId,
					lifeTimeStart,
					typeComponent,
                    pTexture,
                    sz,
                    &pPositionComponent->position,
                    &pAnglePositionComponent->position,
                    &pAnchorPointComponent->m_anchorPoint,
                    &pAnchorPointComponent->m_correctionVector,
                    &pPositionComponent->isDirty);

            renderingData.emplace_back(entityRenderingData);

            unsigned long size = renderingData.size();
            SDL_assert(size>0);

            std::sort(begin(renderingData), end(renderingData), [](auto t1, auto t2){
                return reinterpret_cast<glm::vec3*>(std::get<5>(t1))->z < reinterpret_cast<glm::vec3*>(std::get<5>(t2))->z;
            });
			
            return size-1;

    }

	void RenderingSystem::UnSubscribeEntity(int position){
			std::cout<<"before erase we have : "<<renderingData.size()<<" components"<<std::endl;
			renderingData.erase(renderingData.begin()+position);
			std::cout<<"a components has been reased. There are : "<<renderingData.size()<<" components"<<std::endl;
	}

    unsigned int RenderingSystem::DrawSprites2D() {

        SDLRenderClear();
		int position=0;
        for (auto& [eid, etime, tcomponent, pTexture, pEncodedTextureSize, pvPosition, pvAnglePosition, pvAnchorPoint, pvAnchorPointCorrection, pDirty]: renderingData){
			
			
		    auto encodedTextureSize         = *pEncodedTextureSize;
		    auto pPosition                  = reinterpret_cast<glm::vec3*>  (pvPosition);
		    auto pAnglePosition             = reinterpret_cast<glm::vec3*>  (pvAnglePosition);
		    auto pAnchorPoint               = reinterpret_cast<glm::vec3*>  (pvAnchorPoint);
		    auto pAnchorPointCorrection     = reinterpret_cast<glm::vec3*>  (pvAnchorPointCorrection);

		    //Check if need to render
		    //if (!pTexture) continue;

			//checking
			if(tcomponent==0 ){

				//std::cout<<"pos="<<position<<",id="<<eid<<std::endl;
			}

		    //Render m_pTexture,
		    SDL_Rect dstrect;

		    //AABB
		    dstrect.x = pPosition->x + pAnchorPointCorrection->x;
		    dstrect.y = pPosition->y + pAnchorPointCorrection->y;
		    dstrect.w = encodedTextureSize >> 16;
		    dstrect.h = encodedTextureSize & 0xffff;
			
			
			auto timenow = std::chrono::steady_clock::now();
			auto time_span = timenow-etime;
			double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(timenow - etime).count();

			if(tcomponent==2 ){
				//fades out component
				SDL_SetTextureAlphaMod(pTexture,255-(255*elapsedTime/5000));
			}
			
			if(tcomponent!=2 || (tcomponent==2 && 5000> elapsedTime ) ){
		        SDLRenderCopyEx(pTexture, nullptr, &dstrect, pAnglePosition->z, pAnchorPoint);
		        //SDLRenderCopy(pTexture, nullptr, &dstrect);
		        *pDirty = false;
			}else{
				//UnsuscribeEntity
				UnSubscribeEntity(position);
				position--;
			}
			position++;
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
        DrawSprites2D();     						//DRAW THE SHIPS AND THE FIRE AND THE STUFF IN THE PSCREEN CANVAS
        SDLDetachRenderTexture();                   //NOW IS THE GPU WHERE THE RENDERER WILL DRAW
        SDLRenderCopy(pScreen, nullptr, nullptr);   //TAKE PSCREEN AND COPY IT INTO THE GPU
        SDLUpdateScreen();                          //UPDATE THE SCREEN

    }
}
