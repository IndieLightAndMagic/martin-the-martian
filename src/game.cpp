#include <vector>
#include <functional>

#include <chrono>//change
#include <thread>//change

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
    static unsigned int enemyId;
    static bool bGameIsOn;
    

    static unsigned int  touched;
    static unsigned int  enviromentId;
    static unsigned int  shi;
    static unsigned int  ene;
    static bool xtime;
    static  int positionShip;
    static  int positionEnemy;

    void OnTimerDone();
    void OnEscPressed(const Uint32&, const Sint32&);
    void OnArrowKeyPressed(const Uint32&, const Sint32&);
    //void OnFirePressed(const Uint32&, const Sint32&);
    
    //void moveEnemy(const Uint32&, const Sint32&);
    void moveEnemy();
    void numFire(const Uint32&, const Sint32&);
    void collision();
    void space();


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
                SDLK_DOWN,
                SDLK_z,
                SDLK_x,
                SDLK_c 
        };
        RegisterKeyboardEvents(SDL_KEYDOWN, arrowKeysGroup, OnArrowKeyPressed);
        RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_ESCAPE, OnEscPressed);
        //RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_SPACE, OnFirePressed);
        RegisterKeyboardEvents(SDL_KEYUP, arrowKeysGroup, OnArrowKeyPressed);
        RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_SPACE, numFire);
       // RegisterKeyboardEvents(SDL_KEYDOWN, arrowKeysGroup, moveEnemy);
        //RegisterKeyboardEvents(SDL_KEYUP, arrowKeysGroup, moveEnemy);

         
        
        /* Create Sprite */
        auto shipTexturePath = std::string(RES_DIR)+"ships/goodguy3.png";
        shipId = GTech::Sprite::CreateSprite(shipTexturePath);

        /* Create Background */
        auto backgroundTexturePath = std::string(RES_DIR)+"backgrounds/B0dbg.png";
        backId = GTech::Sprite::CreateSprite(backgroundTexturePath);
        
        /* create enemy */
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

        //enemy
        GTech::Sprite::SetPosition(enemyId, glm::vec3(width >> 2, height >> 2, 5));
        GTech::Sprite::SetScale(enemyId, 0.16);
        ECS::RenderingSystem::SubscribeEntity(enemyId);
        ECS::KinematicsSystem::SubscribeEntity(enemyId);
      
        

    }

    void MainLoop()
    {
        bGameIsOn = true;
        xtime = true;
        ECS::LifeSpanComponent_ x;
        auto signalId = x.onLifeSpanEnded.connect_function(OnTimerDone);
        x.Set(5000);
       

        while (bGameIsOn)
        {
            ECS::UpdateEvents();
            ECS::RenderingSystem::UpdateRenderingSystem();
            ECS::KinematicsSystem::UpdateKinematicsSystem();
            x.Update();
            moveEnemy();
            //collision();
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
       // std::cout << direction << '\n';
       
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
    
   void space(){

      auto [width, height] = SDLWindowSize();
              
      std::cout << "entra  a space \n";
      
      auto& componentManager   = ECS::ComponentManager::GetInstance();
      auto enviroment          = std::string(RES_DIR)+"red_martian64x64.png";
      auto enviromentId        = GTech::Sprite::CreateSprite(enviroment);

      GTech::Sprite::SetPosition(enviromentId, glm::vec3(width >> 2, height >> 5, 5));

      auto enviromentInfo = ECS::ComponentManager::GetInformationComponent(enviromentId);

      ECS::KinematicsSystem::SubscribeEntity(enviromentId);
      ECS::RenderingSystem::SubscribeEntity(enviromentId);

      auto enKinematicTuples                        = enviromentInfo.GetKinematicTuples();
      auto [enPosId, enbackSpeedId, enbackAccelId]  = enKinematicTuples[0];
      auto enviromentSpeedComponent                 = componentManager.GetComponentRaw<ECS::SpeedComponent_>(enbackSpeedId);

      auto direction                                = GAME::GetEntityDirection(componentManager, enviromentInfo);

      auto const maxSpeed = 160.0f;
      auto radians = glm::radians(direction);
     // enviromentSpeedComponent->speed.x = maxSpeed * glm::cos(radians);
      enviromentSpeedComponent->speed.y = maxSpeed * glm::sin(radians);
      enviromentSpeedComponent->speed  *= -1;
    }

       void OnFireEnemy(const uint32_t& kbEvent, const int32_t& kbKey){

        auto resPath = std::string(RES_DIR)+"red_martian64x64.png";
        auto boltId  = GTech::Sprite::CreateSprite(resPath);

        auto boltInfo = ECS::ComponentManager::GetInformationComponent(boltId);
        ECS::KinematicsSystem::SubscribeEntity(boltId);
        ECS::RenderingSystem::SubscribeEntity(boltId);

        auto& componentManager                              = ECS::ComponentManager::GetInstance();
        auto  enemyInformationComponent                      = ECS::ComponentManager::GetInformationComponent(enemyId);
        auto  [posId, anglePositionId, anchorId, textureId] = enemyInformationComponent.GetRenderingTupleIds();
       
        
        //Get Position and Direction of ship
        auto  position  = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId)->position;
        auto  direction = GAME::GetEntityDirection(componentManager, enemyInformationComponent);
        //  std::cout << position[0] << "\n";
        //Set Position of the bolt
        GTech::Sprite::SetPosition(boltId, position);

        //Set Speed of the bolt.
        auto kinematicTuples = boltInfo.GetKinematicTuples();
        auto [boltPosId, boltSpeedId, boltAccelId] = kinematicTuples[0];
        auto speedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(boltSpeedId);

        auto const maxSpeed = -320.0l;
        auto radians = glm::radians(direction);
        speedComponent->speed.x = maxSpeed * glm::cos(radians);
        speedComponent->speed.y = maxSpeed * glm::sin(radians);
 
    }
    
    void OnTimerDone(){
  
       //  xtime=false;
         space();
      //  ExitGame();
    }
    void numFire(const uint32_t& kbEvent, const int32_t& kbKey){
      if(SDLK_SPACE){
          touched++;  
        //  space(4,5);
      }
           if(touched<=100 && xtime==true ){
               OnFirePressed(kbEvent, kbKey);
               OnFireEnemy(kbEvent, kbKey);
               } 
         //else if(touched>100) 
        // std::cout << "tus " << touched << " balas se acabaron "<<std::endl;       
    }

    void ExitGame()
    {
        bGameIsOn = false;
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
        auto  enemyInformationComponent = ECS::ComponentManager::GetInformationComponent(shipId);
        auto angleSpeedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
    
        auto  position  = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId);
        
        positionShip = position->position.z;
       // std::cout<< position->position.y << "\n";

        if (kbKey ==  SDLK_LEFT && kbEvent == SDL_KEYDOWN){
            angleSpeedComponent->speed.z = -45.0f;
        } else if (kbKey == SDLK_RIGHT && kbEvent == SDL_KEYDOWN) {
            angleSpeedComponent->speed.z = +45.0f;
        } else {
            angleSpeedComponent->speed.z = 0.0f;
        }
         if (kbKey == SDLK_UP){
             shi ++;
         }   
        //std::cout << posId << "ship" << "\n";

        if (kbKey == SDLK_UP || (kbKey == SDLK_LEFT && shi >=1) || (kbKey == SDLK_RIGHT && shi >=1) ) {

            auto backInformationComponent               = ECS::ComponentManager::GetInformationComponent(shipId);
            auto backKinematicTuples                    = backInformationComponent.GetKinematicTuples();
            auto [backPosId, backSpeedId, backAccelId]  = backKinematicTuples[0];
            auto backSpeedComponent                     = componentManager.GetComponentRaw<ECS::SpeedComponent_>(backSpeedId);

            auto direction                              = GAME::GetEntityDirection(componentManager, backInformationComponent);

            auto const maxSpeed = 160.0f;
            auto radians = glm::radians(direction);
            backSpeedComponent->speed.x = maxSpeed * glm::cos(radians);
            backSpeedComponent->speed.y = maxSpeed * glm::sin(radians);
            backSpeedComponent->speed  *= 1;
            
          
        }

    }

    void moveEnemy(){ //(const uint32_t& kbEvent, const int32_t& kbKey){
         
        auto& componentManager          = ECS::ComponentManager::GetInstance();
        auto  enemyInformationComponent = ECS::ComponentManager::GetInformationComponent(enemyId);
        auto  kinematicTuples           = enemyInformationComponent.GetKinematicTuples();
        auto  [posId, speedId, accelId] = kinematicTuples[1];

        auto angleSpeedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);

/*
         if (kbKey ==  SDLK_z && kbEvent == SDL_KEYDOWN){
            angleSpeedComponent->speed.z = -45.0f;
        } else if (kbKey == SDLK_c && kbEvent == SDL_KEYDOWN) {
            angleSpeedComponent->speed.z = +45.0f;
        } else {
            angleSpeedComponent->speed.z = 0.0f;
        }
         if (kbKey == SDLK_x){
             ene ++;
         }  */


       angleSpeedComponent->speed.z = -45.0f;
      
       // if (kbKey == SDLK_x || (kbKey == SDLK_z && ene >=1) || (kbKey == SDLK_c && ene >=1) ) {
          
        auto [backPosId, backSpeedId, backAccelId]  = kinematicTuples[0];
        auto backSpeedComponent                     = componentManager.GetComponentRaw<ECS::SpeedComponent_>(backSpeedId);

        auto direction                              = GAME::GetEntityDirection(componentManager, enemyInformationComponent);

        auto const maxSpeed = 160.0f;
        auto radians = glm::radians(direction);
        backSpeedComponent->speed.x = maxSpeed * glm::cos(radians);
        backSpeedComponent->speed.y = maxSpeed * glm::sin(radians);
        backSpeedComponent->speed  *= -1;


        auto  position  = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId);
         positionEnemy = position->position.z;
       //       }
    }

    void collision(){
        
        std::cout << positionEnemy << '\n';
        std::cout << positionShip  << '\n';

      if(positionShip == 0 && positionEnemy == -112)
        ExitGame();

       
    }

};
