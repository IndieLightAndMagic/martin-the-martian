
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

    time_t tiempo;
    long sec;
    long enemyBolt;
    int change;
    static unsigned int backId;
    static unsigned int shipId;
    static unsigned int enemyId;
    static bool bGameIsOn;
    
    void MoveEnemy();
    void OnTimerDone();
    void OnEscPressed(const Uint32&, const Sint32&);
    void OnArrowKeyPressed(const Uint32&, const Sint32&);
    void OnFirePressed(const Uint32&, const Sint32&);
    void OnFireEnemy();
    void Time();

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
        auto shipTexturePath = std::string(RES_DIR)+"ships/goodguy2.png";
        shipId = GTech::Sprite::CreateSprite(shipTexturePath);

        /* Create Background */
        auto backgroundTexturePath = std::string(RES_DIR)+"backgrounds/B1.png";
        backId = GTech::Sprite::CreateSprite(backgroundTexturePath);

        /* Create enemy */
        auto enemyTexturePath = std::string(RES_DIR)+"ships/enemy0.png";
        enemyId = GTech::Sprite::CreateSprite(enemyTexturePath);

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

        //Enemy
        GTech::Sprite::SetPosition(enemyId, glm::vec3(width >> 1 , height >> 3, 5));
        GTech::Sprite::SetScale(enemyId, 1);
        ECS::RenderingSystem::SubscribeEntity(enemyId);
        ECS::KinematicsSystem::SubscribeEntity(enemyId);
        time_t t = time(0); // get time now
        sec = (long)t;
        enemyBolt= (long)t;
        change=-1;
        MoveEnemy();

    }

    void MainLoop()
    {
        bGameIsOn = true;
        /*
        ECS::LifeSpanComponent_ x;
        auto signalId = x.onLifeSpanEnded.connect_function(OnTimerDone);
        x.Set(6000);
        */

        while (bGameIsOn)
        {
            ECS::UpdateEvents();
            ECS::RenderingSystem::UpdateRenderingSystem();
            ECS::KinematicsSystem::UpdateKinematicsSystem();
            Time();
            OnFireEnemy();
            //x.Update();
        }
        //x.onLifeSpanEnded.disconnect(signalId);

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

        auto angleSpeedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);

        if (kbKey ==  SDLK_LEFT && kbEvent == SDL_KEYDOWN){
            angleSpeedComponent->speed.z = -45.0f;
        } else if (kbKey == SDLK_RIGHT && kbEvent == SDL_KEYDOWN) {
            angleSpeedComponent->speed.z = +45.0f;
        } else {
            angleSpeedComponent->speed.z = 0.0f;
        }

        //if (kbKey == SDLK_UP) {

            auto backInformationComponent               = ECS::ComponentManager::GetInformationComponent(backId);
            auto backKinematicTuples                    = backInformationComponent.GetKinematicTuples();
            auto [backPosId, backSpeedId, backAccelId]  = backKinematicTuples[0];
            auto backSpeedComponent                     = componentManager.GetComponentRaw<ECS::SpeedComponent_>(backSpeedId);

            auto direction                              = GAME::GetEntityDirection(componentManager, shipInformationComponent);

            auto const maxSpeed = 160.0f;
            auto radians = glm::radians(direction);
            backSpeedComponent->speed.x = maxSpeed * glm::cos(radians);
            backSpeedComponent->speed.y = maxSpeed * glm::sin(radians);
            backSpeedComponent->speed  *= -1;

        //}

    }
    void MoveEnemy()
    {
        
        auto& componentManager          = ECS::ComponentManager::GetInstance();
        auto  enemyInformationComponent1  = ECS::ComponentManager::GetInformationComponent(enemyId);
        auto  kinematicTuples           = enemyInformationComponent1.GetKinematicTuples();
        auto  [posId, speedId, accelId] = kinematicTuples[1];

        auto angleSpeedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
        angleSpeedComponent->speed.z = +0.0f;
    
        auto enemyInformationComponent               = ECS::ComponentManager::GetInformationComponent(enemyId);
        auto enemyKinematicTuples                    = enemyInformationComponent.GetKinematicTuples();
        auto [enemyPosId, enemySpeedId, enemyAccelId]  = enemyKinematicTuples[0];
        auto enemySpeedComponent                     = componentManager.GetComponentRaw<ECS::SpeedComponent_>(enemySpeedId);

        auto direction                              = GAME::GetEntityDirection(componentManager, enemyInformationComponent1);

        auto const maxSpeed = 50.0f;
        auto radians = glm::radians(direction);
        enemySpeedComponent->speed.x = maxSpeed * glm::sin(radians);
        enemySpeedComponent->speed.y = maxSpeed * glm::cos(radians);
        enemySpeedComponent->speed  *= +1; // change to adress enemy  
         
    }

        // bolt the enemy
    void OnFireEnemy()
    {
        time_t t = time(0); // get time now
        long enemyBolt2= (long)t;
        if((enemyBolt2 - enemyBolt)>=0.5){
            auto resPath = std::string(RES_DIR)+"orangebolt.png";
            auto boltId = GTech::Sprite::CreateSprite(resPath);
            auto boltInfo = ECS::ComponentManager::GetInformationComponent(boltId);
            ECS::KinematicsSystem::SubscribeEntity(boltId);
            ECS::RenderingSystem::SubscribeEntity(boltId);

            auto& componentManager                              = ECS::ComponentManager::GetInstance();
            auto  enemyInformationComponent                      = ECS::ComponentManager::GetInformationComponent(enemyId);
            auto  [posId, anglePositionId, anchorId, textureId] = enemyInformationComponent.GetRenderingTupleIds();

            //Get Position and Direction of ship
            auto  position  = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId)->position;
            auto  direction = GAME::GetEntityDirection(componentManager, enemyInformationComponent);

            //Set Position of the bolt
            GTech::Sprite::SetPosition(boltId, position);

            //Set Speed of the bolt.
            auto kinematicTuples = boltInfo.GetKinematicTuples();
            auto [boltPosId, boltSpeedId, boltAccelId] = kinematicTuples[0];
            auto speedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(boltSpeedId);

            auto const maxSpeed = 920.0l;
            auto radians = glm::radians(direction);
            speedComponent->speed.x = maxSpeed * glm::cos(radians);
            speedComponent->speed.y = maxSpeed * glm::sin(radians);
            speedComponent->speed *= -1;
        }
        enemyBolt = enemyBolt2;        
    }

    void Time()
    {
        time_t t = time(0); // get time now
        long sec2 = (long)t;
        if((sec2 - sec )>=10) {
            auto& componentManager          = ECS::ComponentManager::GetInstance();
            auto  enemyInformationComponent1  = ECS::ComponentManager::GetInformationComponent(enemyId);
            auto  kinematicTuples           = enemyInformationComponent1.GetKinematicTuples();
            auto  [posId, speedId, accelId] = kinematicTuples[1];

            auto angleSpeedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
            angleSpeedComponent->speed.z = +0.0f;
        
            auto enemyInformationComponent               = ECS::ComponentManager::GetInformationComponent(enemyId);
            auto enemyKinematicTuples                    = enemyInformationComponent.GetKinematicTuples();
            auto [enemyPosId, enemySpeedId, enemyAccelId]  = enemyKinematicTuples[0];
            auto enemySpeedComponent                     = componentManager.GetComponentRaw<ECS::SpeedComponent_>(enemySpeedId);

            auto direction                              = GAME::GetEntityDirection(componentManager, enemyInformationComponent1);

            auto const maxSpeed = 50.0f;
            auto radians = glm::radians(direction);
            enemySpeedComponent->speed.x = maxSpeed * glm::sin(radians);
            enemySpeedComponent->speed.y = maxSpeed * glm::cos(radians);
            enemySpeedComponent->speed  *= change; // change to adress enemy  

            sec= sec2;
            change *= -1;
            
        }
    }

};
