#include <vector>
#include <functional>

#include <glm/detail/func_trigonometric.hpp>
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

    void OnTimerDone();
    void OnEscPressed(const Uint32&, const Sint32&);
    void OnArrowKeyPressed(const Uint32&, const Sint32&);
    void OnFirePressed(const Uint32&, const Sint32&);


    float GetEntityDirection(const ECS::ComponentManager &componentManager, const ECS::EntityInformationComponent_ &informationComponent)
    {
        auto  [posId, anglePositionId, anchorId, textureId] = informationComponent.GetRenderingTupleIds();

        //Get Position and Direction of ship
        auto  position  = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId)->position;
        auto  direction = componentManager.GetComponentRaw<ECS::PositionComponent_>(anglePositionId)->position.z;
        direction -= 90.0f;
        direction -= (direction >= 360.0f) ? 360.0f : 0.0f;
        direction += (direction  < 0.0f  ) ? 360.0f : 0.0f;

        return direction;

    }

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
        RegisterKeyboardEvents(SDL_KEYUP, arrowKeysGroup, OnArrowKeyPressed);


        /* Create Sprite */
        auto shipTexturePath = std::string(RES_DIR)+"ships/goodguy3.png";
        shipId = GTech::Sprite::CreateSprite(shipTexturePath);

        /* Create Background */
        auto backgroundTexturePath = std::string(RES_DIR)+"backgrounds/B0dbg.png";
        backId = GTech::Sprite::CreateSprite(backgroundTexturePath);

        /* Dimensions */
        auto [width, height] = SDLWindowSize();

        /* Init Systems */
        ECS::RenderingSystem::InitRenderingSystem();
        ECS::KinematicsSystem::InitKinematicsSystem();

        //Subscribe Entities into Systems

        //Ship
        GTech::Sprite::SetPosition(shipId, glm::vec3(width >> 1, height >> 1, 5));
        GTech::Sprite::SetScale(shipId, 0.16);
        ECS::RenderingSystem::SubscribeEntity(shipId);
        ECS::KinematicsSystem::SubscribeEntity(shipId);

        //Background
        ECS::RenderingSystem::SubscribeEntity(backId);
        ECS::KinematicsSystem::SubscribeEntity(backId);
        GTech::Sprite::SetPosition(backId, glm::vec3(width >> 1, height >> 1, 0));

       


    }

    void MainLoop()
    {
        bGameIsOn = true;

        ECS::LifeSpanComponent_ x;
        auto signalId = x.onLifeSpanEnded.connect_function(OnTimerDone);
        x.Set(6000);


        while (bGameIsOn)
        {
            ECS::UpdateEvents();
            ECS::RenderingSystem::UpdateRenderingSystem();
            ECS::KinematicsSystem::UpdateKinematicsSystem();
            x.Update();
        }
        x.onLifeSpanEnded.disconnect(signalId);

    }

    void OnFirePressed(const uint32_t& kbEvent, const int32_t& kbKey){

        auto resPath = std::string(RES_DIR)+"purplebolt16x16.png";
        auto boltId = GTech::Sprite::CreateSprite(resPath);

        auto boltInfo = ECS::ComponentManager::GetInformationComponent(boltId);
        ECS::KinematicsSystem::SubscribeEntity(boltId);
        ECS::RenderingSystem::SubscribeEntity(boltId);

        auto& componentManager                              = ECS::ComponentManager::GetInstance();
        auto  shipInformationComponent                      = ECS::ComponentManager::GetInformationComponent(shipId);
        auto  [posId, anglePositionId, anchorId, textureId] = shipInformationComponent.GetRenderingTupleIds();

        //Get Position and Direction of ship
        auto  position  = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId)->position;
        auto  direction = GAME::GetEntityDirection(componentManager, shipInformationComponent);

        //Set Position of the bolt
        GTech::Sprite::SetPosition(boltId, position);

        //Set Speed of the bolt.
        auto kinematicTuples = boltInfo.GetKinematicTuples();
        auto [boltPosId, boltSpeedId, boltAccelId] = kinematicTuples[0];
        auto speedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(boltSpeedId);

        auto const maxSpeed = 320.0l;
        auto radians = glm::radians(direction);
        speedComponent->speed.x = maxSpeed * glm::cos(radians);
        speedComponent->speed.y = maxSpeed * glm::sin(radians);

    }
    void ExitGame()
    {
        bGameIsOn = false;
    }

    void OnTimerDone(){
        //ExitGame();
    }

    void OnEscPressed(const Uint32& kbEvent, const Sint32& kbKey){

        std::cout << "GAME::OnEscPressed "  << __FUNCTION__ << std::endl;
        ExitGame();
    }
    
        void OnArrowKeyPressed(const Uint32& kbEvent, const Sint32& kbKey){

        auto& componentManager          = ECS::ComponentManager::GetInstance();
        auto  shipInformationComponent  = ECS::ComponentManager::GetInformationComponent(shipId);
        auto  kinematicTuples           = shipInformationComponent.GetKinematicTuples();
        auto  [posId, speedId, accelId] = kinematicTuples[1];

        auto  backgroundInformationComponent  = ECS::ComponentManager::GetInformationComponent(backId);
        auto  kinematicTuplesBackground       =  backgroundInformationComponent.GetKinematicTuples ();
        auto  [posbId, speedbId, accelbId] = kinematicTuplesBackground[1];


        auto angleSpeedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
        auto backgroundPosComponent = componentManager.GetComponentRaw<ECS::PositionComponent_>(posbId);

        if (kbKey ==  SDLK_LEFT && kbEvent == SDL_KEYDOWN){
            //angleSpeedComponent->speed.z = -45.0f;
            backgroundPosComponent->position.z -=(-45.0f);
        } else if (kbKey == SDLK_RIGHT && kbEvent == SDL_KEYDOWN) {
            //angleSpeedComponent->speed.z = +45.0f;
            backgroundPosComponent->position.z +=(+45.0f);
        } else {
            //angleSpeedComponent->speed.z = 0.0f;
        }

    }

};