
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
    static unsigned int enemyId;
    static bool bGameIsOn;
    

    static unsigned int  touched;
    static unsigned int  enviromentId;
    static unsigned int  shi;
    static unsigned int  ene;
    static bool xtime;
    
    static unsigned int boltX;
    static unsigned int boltY;
    static unsigned int enemyX;
    static unsigned int enemyY; 
    static unsigned int enemySpeedY;
    static unsigned int boltSpeedY;
    SDL_Joystick *joy;
    static uint Xbutton;

    

    void OnTimerDone();
    void OnEscPressed(const Uint32&, const Sint32&);
    void OnArrowKeyPressed(const Uint32&, const Sint32&);
    void OnFirePressed(const Uint32&, const Sint32&);
    //void moveEnemy(const Uint32&, const Sint32&);
    void moveEnemy();
    //void numFire(const Uint32&, const Sint32&);
    void collision();
    


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
        std::cout << posId << '\n' ;
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
        RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_SPACE, OnFirePressed);
        RegisterKeyboardEvents(SDL_KEYUP, arrowKeysGroup, OnArrowKeyPressed);
       // RegisterKeyboardEvent(SDL_KEYDOWN, SDLK_SPACE, numFire);
       // RegisterKeyboardEvents(SDL_KEYDOWN, arrowKeysGroup, moveEnemy);
       // RegisterKeyboardEvents(SDL_KEYUP, arrowKeysGroup, moveEnemy);

         
        
        /* Create Sprite */
        auto shipTexturePath = std::string(RES_DIR)+"ships/goodguy3.png";
        shipId = GTech::Sprite::CreateSprite(shipTexturePath);

        /* Create Background */
        auto backgroundTexturePath = std::string(RES_DIR)+"backgrounds/B0dbg.png";
        backId = GTech::Sprite::CreateSprite(backgroundTexturePath);
        
        // create enemy 
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
        GTech::Sprite::SetPosition(enemyId, glm::vec3(width >> 1, height >> 4, 5));
        GTech::Sprite::SetScale(enemyId, 0.16);
        ECS::RenderingSystem::SubscribeEntity(enemyId);
        ECS::KinematicsSystem::SubscribeEntity(enemyId);
                  moveEnemy();

        

    }

    void MainLoop()
    {   
    
        bGameIsOn = true;
        xtime = true;
        ECS::LifeSpanComponent_ x;
        auto signalId = x.onLifeSpanEnded.connect_function(OnTimerDone);
        x.Set(5000);

// Initialize the joystick subsystem


        while (bGameIsOn)
        {
            ECS::UpdateEvents();
            ECS::RenderingSystem::UpdateRenderingSystem();
            ECS::KinematicsSystem::UpdateKinematicsSystem();
            x.Update();
            moveEnemy();
            collision();       

           
        }
        // std::cout << joy << '\n';
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
        auto kinematicTuples                       = boltInfo.GetKinematicTuples();
        auto [boltPosId, boltSpeedId, boltAccelId] = kinematicTuples[0];
        auto speedComponent                        = componentManager.GetComponentRaw<ECS::SpeedComponent_>(boltSpeedId);

        auto backpos  = componentManager.GetComponentRaw<ECS::PositionComponent_>(boltPosId);

        auto const maxSpeed = 320.0l;
        auto radians = glm::radians(direction);
        speedComponent->speed.x = maxSpeed * glm::cos(radians);
        speedComponent->speed.y = maxSpeed * glm::sin(radians);

          boltX = backpos->position.x;
          boltY = backpos->position.y;
         // std::cout << speedComponent->speed.x << "shipx \n";
         // std::cout << speedComponent->speed.y << "shipyy \n";
          boltSpeedY =  abs(speedComponent->speed.y);
          collision();
          ene++;
    }  
    
    void OnTimerDone(){
  
      //  ExitGame();
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

        auto angleSpeedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
    
        //auto  position  = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId);
        
        //std::cout << position <<'\n' ;
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
        if (kbKey == SDLK_UP || (kbKey == SDLK_LEFT && shi >=1) || (kbKey == SDLK_RIGHT && shi >=1) ) {

            auto backInformationComponent               = ECS::ComponentManager::GetInformationComponent(backId);
            auto backKinematicTuples                    = backInformationComponent.GetKinematicTuples();
            auto [backPosId, backSpeedId, backAccelId]  = backKinematicTuples[0];
            auto backSpeedComponent                     = componentManager.GetComponentRaw<ECS::SpeedComponent_>(backSpeedId);
            auto backpos                                = componentManager.GetComponentRaw<ECS::PositionComponent_>(backPosId);

            auto direction                              = GAME::GetEntityDirection(componentManager, shipInformationComponent);

            auto const maxSpeed = 160.0f;
            auto radians = glm::radians(direction);
            backSpeedComponent->speed.x = maxSpeed * glm::cos(radians);
            backSpeedComponent->speed.y = maxSpeed * glm::sin(radians);
            backSpeedComponent->speed  *= -1;

          
          // std::cout << backpos->position.x << " positionx \n";
          // std::cout << backpos->position.y << " positiony \n";



        }

    }

    void moveEnemy(){ //const Uint32& kbEvent, const Sint32& kbKey ){

        auto& componentManager          = ECS::ComponentManager::GetInstance();
        auto  enemyInformationComponent = ECS::ComponentManager::GetInformationComponent(enemyId);
        auto  kinematicTuples           = enemyInformationComponent.GetKinematicTuples();
        auto  [posId, speedId, accelId] = kinematicTuples[1];

        auto angleSpeedComponent = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
    
        auto  position  = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId);
        
       // angleSpeedComponent->speed.z = +40.0f;

        /*if (kbKey ==  SDLK_z && kbEvent == SDL_KEYDOWN){
            angleSpeedComponent->speed.z = -45.0f;
        } else if (kbKey == SDLK_c && kbEvent == SDL_KEYDOWN) {
            angleSpeedComponent->speed.z = +45.0f;
        } else {
            angleSpeedComponent->speed.z = 0.0f;
        }
         if (kbKey == SDLK_x){
             shi ++;
         }   */
       // if (kbKey == SDLK_x || (kbKey == SDLK_z && shi >=1) || (kbKey == SDLK_c && shi >=1) ) {

            auto backInformationComponent               = ECS::ComponentManager::GetInformationComponent(enemyId);
            auto backKinematicTuples                    = backInformationComponent.GetKinematicTuples();
            auto [backPosId, backSpeedId, backAccelId]  = backKinematicTuples[0];
            auto backSpeedComponent                     = componentManager.GetComponentRaw<ECS::SpeedComponent_>(backSpeedId);
            auto backpos                                = componentManager.GetComponentRaw<ECS::PositionComponent_>(backPosId);
            auto direction                              = GAME::GetEntityDirection(componentManager, enemyInformationComponent);
           
            auto const maxSpeed = 20.0f;
            auto radians = glm::radians(direction);
            backSpeedComponent->speed.x = maxSpeed * glm::cos(radians);
            backSpeedComponent->speed.y = maxSpeed * glm::sin(radians);
            backSpeedComponent->speed  *= -1;

           // std::cout << backSpeedComponent->speed.x << "enemyx \n";
           // std::cout << backSpeedComponent->speed.y << "enemyy \n";
           // std::cout << backpos->position.x << "position \n";
          //  std::cout << backpos->position.y << "position \n";

            enemyX      = backpos->position.x;
            enemyY      = backpos->position.y;
            enemySpeedY = backSpeedComponent->speed.y ;


            
       // }

    }
    void collision(){
       /*  
         std::cout << boltSpeedY << " positionbala \n";
         std::cout << boltY << " positionbala \n";
         std::cout << enemySpeedY << " positionenemy \n";
         std::cout << enemyY << " positionenemy \n";
        */

        if(ene>=1 && (enemySpeedY-boltSpeedY) == (350*enemySpeedY*boltSpeedY)/enemySpeedY ){
        std::cout << "muerto \n";
        ExitGame();
         }
       
    }
    
};
