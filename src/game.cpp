
#include <vector>
#include <functional>

#include <SDL2/SDL.h>
#include <sdlwrapper.h>

#include <ECS/System/kinematics.h>
#include <ECS/System/rendering.h>
#include <ECS/Event/events.h>

#include <Sprite.h>

using namespace std;

void RegisterKeyboardEvent(Uint32 event, Sint32 key, function<void(const Uint32&, const Sint32&)> slot);
void RegisterKeyboardEvents(Uint32 event, vector<Sint32>& keys, std::function<void(const Uint32&, const Sint32&)> slot);



namespace GAME{

    static unsigned int backId;
    static unsigned int shipId;
    static bool bGameIsOn;

    void OnEscPressed(const Uint32&, const Sint32&);
    void OnArrowKeyPressed(const Uint32&, const Sint32&);
    void OnFirePressed(const Uint32&, const Sint32&);


    void GameInitialization()
    {

        //My Keyboard slots
        auto arrowKeysGroup = std::vector<Sint32>{
                SDLK_RIGHT,
                SDLK_LEFT,
                SDLK_UP,
                SDLK_DOWN
        };
        RegisterKeyboardEvents(SDL_KEYDOWN, arrowKeysGroup, OnArrowKeyPressed);
        RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_ESCAPE, OnEscPressed);
        RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_SPACE, OnFirePressed);


        /* Create Sprite */
        auto shipTexturePath = std::string(RES_DIR)+"ships/goodguy0.png";
        shipId = GTech::Sprite::CreateSprite(shipTexturePath);

        /* Create Background */
        auto backgroundTexturePath = std::string(RES_DIR)+"backgrounds/B0.png";
        backId = GTech::Sprite::CreateSprite(backgroundTexturePath);

        /* Dimensions */
        auto [width, height] = SDLWindowSize();
        GTech::Sprite::SetPosition(shipId, glm::vec3(width >> 1, height >> 1, 5));
        GTech::Sprite::SetScale(shipId, 0.16);
        /* Init Systems */
        ECS::RenderingSystem::InitRenderingSystem();
        ECS::KinematicsSystem::InitKinematicsSystem();

        //Subscribe Entities into Systems

        //Ship
        ECS::RenderingSystem::SubscribeEntity(shipId);
        ECS::KinematicsSystem::SubscribeEntity(shipId);

        //Background
        ECS::RenderingSystem::SubscribeEntity(backId);


    }

    void MainLoop()
    {
        bGameIsOn = true;
        while (bGameIsOn)
        {
            ECS::UpdateEvents();
            ECS::RenderingSystem::UpdateRenderingSystem();
            ECS::KinematicsSystem::UpdateKinematicsSystem();
        }
    }

    void OnFirePressed(const uint32_t& kbEvent, const int32_t& kbKey){

        auto resPath = std::string(RES_DIR)+"purplebolt16x16.png";
        auto boltId = GTech::Sprite::CreateSprite(resPath);
        auto boltInfo = ECS::ComponentManager::GetInformationComponent(boltId);
        ECS::KinematicsSystem::SubscribeEntity(boltId);
        ECS::RenderingSystem::SubscribeEntity(boltId);

        auto& componentManager          = ECS::ComponentManager::GetInstance();
        auto  shipInformationComponent  = ECS::ComponentManager::GetInformationComponent(shipId);
        auto  [posId, speedId, accelId] = shipInformationComponent.GetKinematicTupleIds();

        GTech::Sprite::SetPosition(boltId, componentManager.GetComponentRaw<ECS::PositionComponent_>(posId)->position);

        auto [boltPosId, boltSpeedId, boltAccelId] = boltInfo.GetKinematicTupleIds();
        auto speedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(boltSpeedId);
        speedComponent->speed.y = -320.0f;

    }

    void OnEscPressed(const Uint32& kbEvent, const Sint32& kbKey){

        std::cout << "GAME::OnEscPressed "  << __FUNCTION__ << std::endl;
        bGameIsOn = false;
    }

    void OnArrowKeyPressed(const Uint32& kbEvent, const Sint32& kbKey){

        auto& componentManager          = ECS::ComponentManager::GetInstance();
        auto  shipInformationComponent  = ECS::ComponentManager::GetInformationComponent(shipId);
        auto  [posId, speedId, accelId] = shipInformationComponent.GetKinematicTupleIds();

        auto positionComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);


        if (kbKey ==  SDLK_LEFT){
            positionComponent->speed.x = -45.0f;
        } else if (kbKey == SDLK_RIGHT) {
            positionComponent->speed.x = +45.0f;
        } else if (kbKey == SDLK_DOWN) {
        } else if (kbKey == SDLK_UP) {
        }

    }

};
