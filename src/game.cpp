
#include <vector>
#include <functional>
#include <ctime>
#include <chrono>
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
    static unsigned int enemyShipId;
    static bool bGameIsOn;
    static bool bEnemyDirection;
    static long lastTimeEnemyMove;
    static long lastTimeEnemyShot;

    void OnTimerDone();
    void OnEscPressed(const Uint32&, const Sint32&);
    void OnArrowKeyPressed(const Uint32&, const Sint32&);
    void OnFirePressed(const Uint32&, const Sint32&);

    void OnMoveEnemy();
    void OnEnemyShot();

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

        /* Create Sprite enemy*/
        auto enemyShipTexturePath = std::string(RES_DIR)+"ships/enemy1.png";
        enemyShipId = GTech::Sprite::CreateSprite(enemyShipTexturePath);

        /* Create Background */
        auto backgroundTexturePath = std::string(RES_DIR)+"backgrounds/B0.png";
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
        ECS::RenderingSystem::SubscribeEntity(shipId, false);
        ECS::KinematicsSystem::SubscribeEntity(shipId);

        //Background
        ECS::RenderingSystem::SubscribeEntity(backId, false);
        ECS::KinematicsSystem::SubscribeEntity(backId);
        GTech::Sprite::SetPosition(backId, glm::vec3(width >> 1, height >> 1, 0));

        //Enemy Ship
        GTech::Sprite::SetPosition(enemyShipId, glm::vec3(width >> 4, height >> 4, 5));
        GTech::Sprite::SetScale(enemyShipId, 0.16);
        ECS::RenderingSystem::SubscribeEntity(enemyShipId, false);
        ECS::KinematicsSystem::SubscribeEntity(enemyShipId);

        auto& componentManager                         = ECS::ComponentManager::GetInstance();
        auto  enemyShipInformationComponent            = ECS::ComponentManager::GetInformationComponent(enemyShipId);
        auto  enemyKinematicTuples                     = enemyShipInformationComponent.GetKinematicTuples();
        auto  [enemyPosId, enemySpeedId, enemyAccelId] = enemyKinematicTuples[1];
        auto  [enemyPos0Id, enemySpeed0Id, enemyAccel0Id] = enemyKinematicTuples[0];

        auto  enemyPosComponent                        = componentManager.GetComponentRaw<ECS::PositionComponent_>(enemyPosId);
        enemyPosComponent->position.z =180.0f;

        auto enemySpeedComponent                       = componentManager.GetComponentRaw<ECS::SpeedComponent_>(enemySpeed0Id);
        auto enemyDirection                            = GAME::GetEntityDirection(componentManager, enemyShipInformationComponent);
        auto const maxSpeed = 100.0f;
        auto radians = glm::radians(enemyDirection);
        enemySpeedComponent->speed.x = maxSpeed * glm::sin(radians);
        enemySpeedComponent->speed.y = maxSpeed * glm::cos(radians);

        bEnemyDirection = false;
        std::time_t t = std::time(0);
        lastTimeEnemyMove = (long)t;
    }

    void MainLoop()
    {
        bGameIsOn = true;

        ECS::LifeSpanComponent_ x;
        auto signalId = x.onLifeSpanEnded.connect_function(OnTimerDone);
        x.Set(5000);


        while (bGameIsOn)
        {
            ECS::UpdateEvents();
            ECS::RenderingSystem::UpdateRenderingSystem();
            ECS::KinematicsSystem::UpdateKinematicsSystem();
            x.Update();
            OnMoveEnemy();
            OnEnemyShot();
        }
        x.onLifeSpanEnded.disconnect(signalId);

    }

    void OnFirePressed(const uint32_t& kbEvent, const int32_t& kbKey){
            
            auto resPath = std::string(RES_DIR)+"purplebolt16x16.png";
            auto boltId = GTech::Sprite::CreateSprite(resPath);
            
            auto boltInfo = ECS::ComponentManager::GetInformationComponent(boltId);
            ECS::KinematicsSystem::SubscribeEntity(boltId);
            ECS::RenderingSystem::SubscribeEntity(boltId, true);
            
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

    }

    void OnEscPressed(const Uint32& kbEvent, const Sint32& kbKey){

        std::cout << "GAME::OnEscPressed "  << __FUNCTION__ << std::endl;
        ExitGame();
    }

    void OnArrowKeyPressed(const Uint32& kbEvent, const Sint32& kbKey){

        auto& componentManager                       = ECS::ComponentManager::GetInstance();
        auto  shipInformationComponent               = ECS::ComponentManager::GetInformationComponent(shipId);
        auto  backInformationComponent               = ECS::ComponentManager::GetInformationComponent(backId);
        auto  kinematicTuples                        = shipInformationComponent.GetKinematicTuples();
        auto  backKinematicTuples                    = backInformationComponent.GetKinematicTuples();
        auto  [posId, speedId, accelId]              = kinematicTuples[1];
        auto  [backPosId, backSpeedId, backAccelId]  = backKinematicTuples[1];
        auto angleSpeedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
        auto backPosComponent = componentManager.GetComponentRaw<ECS::PositionComponent_>(backPosId);


        if (kbKey ==  SDLK_LEFT && kbEvent == SDL_KEYDOWN){
            backPosComponent->position.z -=(-45.0f);
        } else if (kbKey == SDLK_RIGHT && kbEvent == SDL_KEYDOWN) {
            backPosComponent->position.z +=(+45.0f);
        } else {
            angleSpeedComponent->speed.z = 0.0f;
        }
    }


    void OnMoveEnemy(){

        auto& componentManager                            = ECS::ComponentManager::GetInstance();
        auto  enemyShipInformationComponent               = ECS::ComponentManager::GetInformationComponent(enemyShipId);
        auto  enemyKinematicTuples                        = enemyShipInformationComponent.GetKinematicTuples();

        auto [enemyPosId, enemySpeedId, enemyAccelId]     = enemyKinematicTuples[0];
        auto [enemyPos0Id, enemySpeed0Id, enemyAccel0Id]  = enemyKinematicTuples[1];
        auto enemySpeedComponent                          = componentManager.GetComponentRaw<ECS::SpeedComponent_>(enemySpeedId);

        auto const maxSpeed = 100.0f;

        std::time_t t = std::time(0);
        long actualTime = (long)t;

        if ((actualTime - lastTimeEnemyMove) > 10 ){
            auto enemyDirection                            = GAME::GetEntityDirection(componentManager, enemyShipInformationComponent);
            auto radians = glm::radians(enemyDirection);

            lastTimeEnemyMove = actualTime;
            if (bEnemyDirection){
                radians = glm::radians(enemyDirection);
                enemySpeedComponent->speed.x = maxSpeed * glm::sin(radians);
                enemySpeedComponent->speed.y = maxSpeed * glm::cos(radians);
                bEnemyDirection = false;
            }
            else{
                radians = glm::radians(enemyDirection*-1);
                enemySpeedComponent->speed.x = maxSpeed * glm::sin(radians);
                enemySpeedComponent->speed.y = maxSpeed * glm::cos(radians);
                bEnemyDirection = true;
            }
        }
    }


    void OnEnemyShot(){

        std::time_t t = std::time(0);
        long actualTime = (long)t;

        if ((actualTime - lastTimeEnemyShot) > 1) {

            auto resPath = std::string(RES_DIR)+"orangebolt.png";
            auto boltId = GTech::Sprite::CreateSprite(resPath);

            auto boltInfo = ECS::ComponentManager::GetInformationComponent(boltId);
            ECS::KinematicsSystem::SubscribeEntity(boltId);
            ECS::RenderingSystem::SubscribeEntity(boltId, true);

            auto& componentManager                              = ECS::ComponentManager::GetInstance();
            auto  shipInformationComponent                      = ECS::ComponentManager::GetInformationComponent(enemyShipId);
            auto  [posId, anglePositionId, anchorId, textureId] = shipInformationComponent.GetRenderingTupleIds();

            //Get Position and Direction of enemy ship
            auto  position  = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId)->position;
            auto  direction = GAME::GetEntityDirection(componentManager, shipInformationComponent);

            //Set Position of the bolt
            GTech::Sprite::SetPosition(boltId, position);

            //Set Speed of the bolt.
            auto kinematicTuples = boltInfo.GetKinematicTuples();
            auto [boltPosId, boltSpeedId, boltAccelId] = kinematicTuples[0];
            auto speedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(boltSpeedId);

            auto const maxSpeed = 320.0l;
            auto radians = glm::radians(direction-14);

            if (bEnemyDirection)  radians = glm::radians(direction+14);

            speedComponent->speed.x = maxSpeed * glm::cos(radians);
            speedComponent->speed.y = maxSpeed * glm::sin(radians);
            lastTimeEnemyShot = actualTime;
        }
    }

};
