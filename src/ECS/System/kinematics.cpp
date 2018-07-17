#include <ECS/System/kinematics.h>
#include <glm/detail/func_common.hpp>

using namespace ECS;


std::vector<unsigned int>	KinematicsSystem::ids{};
std::vector<glm::vec3*>		KinematicsSystem::positions{};
std::vector<glm::vec3*>		KinematicsSystem::speeds{};
std::vector<glm::vec3*>		KinematicsSystem::accelerations{};
std::vector<bool*>          KinematicsSystem::positionDirtyFlags{};
glm::vec3**                 KinematicsSystem::positions_ = nullptr;
glm::vec3**                 KinematicsSystem::speeds_ = nullptr;
glm::vec3**                 KinematicsSystem::accelerations_ = nullptr;
bool**                      KinematicsSystem::positionDirtyFlags_ = nullptr;

void KinematicsSystem::InitKinematicsSystem(){

}

unsigned int KinematicsSystem::SubscribeEntity(unsigned int entityId){

    auto& entityManager      = ECS::EntityManager::GetInstance();
    auto& componentManager   = ECS::ComponentManager::GetInstance();
    auto componentVectors   = entityManager.GetComponentsIds(entityId);

    //CONVENTION: the first component of ANY entity is the EntityInformationComponent_
    auto entityInfo         = componentVectors[0];
    auto entityInfoRP       = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityInfo);

    //Kinematic Triad
    auto kinematicTuples            = entityInfoRP->GetKinematicTuples();

    for ( auto [posId, speedId, accelId] : kinematicTuples)
    {
        auto pSpeedComponent            = componentManager.GetComponentRaw<ECS::SpeedComponent_>(speedId);
        auto pPositionComponent         = componentManager.GetComponentRaw<ECS::PositionComponent_>(posId);
        auto pAccelerationComponent     = componentManager.GetComponentRaw<ECS::AccelerationComponent_>(accelId);

        SDL_assert(pSpeedComponent!= nullptr && pPositionComponent != nullptr && pAccelerationComponent != nullptr);


        ids.push_back(entityId);
        accelerations.push_back(&pAccelerationComponent->acceleration);
        accelerations_  = accelerations.data();
        speeds.push_back(&pSpeedComponent->speed);
        speeds_         = speeds.data();
        positions.push_back(&pPositionComponent->position);
        positions_      = positions.data();
        positionDirtyFlags.push_back(&pPositionComponent->isDirty);
        positionDirtyFlags_ = positionDirtyFlags.data();

    }

    return 1;

}

void KinematicsSystem::UpdateKinematicsSystem(){

    //x(t) = 0.5 * akte * t**2 + s*t + xo
    //s(t) = akte*t + vo
    //a(t) = akte
    static const auto TICKS_PER__SEC = SDL_GetPerformanceFrequency();
    static const auto SECS_PER__TICK = 1.0f / TICKS_PER__SEC;

    ///FPS DELIMITER
    static const auto FPS_HZ_____MAX = 30.0f;
    static const auto FPS_MSECS__MIN = 1000.0f / FPS_HZ_____MAX;
    static const auto FPS_SECS___MIN = 1.0f / FPS_HZ_____MAX;
    static const auto fFPS_TICKS_MIN = TICKS_PER__SEC * FPS_SECS___MIN;
    static const auto FPS_TICKS__MIN = static_cast<uint64_t>(fFPS_TICKS_MIN);

    auto tNow = SDL_GetTicks();
    static auto tBefore = tNow;
    
    auto tDelta = static_cast<float>(tNow - tBefore);
    tDelta /= 1000.0f;
    if (tNow == tBefore){
        return;
    } else {
        tBefore = tNow;
    }
    SDL_assert(tDelta > 0.0f);
    
    auto& dt    = tDelta;
    auto sz     = ids.size();
    auto dt2    = dt * dt;

    auto kinematic  = [&](int index){

        auto speed  = speeds_[index]; if (speed->y == 0.0f && speed->x == 0.0f && speed->z == 0.0f) return;
        
        auto accel  = accelerations_[index];
        auto& pos   = *(positions_[index]);
        auto& dirtyFlag = *(positionDirtyFlags_[index]);

        glm::vec3 accelDelta(0.5 * accel->x * dt2, 0.5 * accel->y * dt2, 0.5 * accel->z * dt2);
        glm::vec3 speedDelta(speed->x * dt , speed->y * dt, speed->z * dt);
        pos += speedDelta;
        
        auto xmoved = glm::abs(speed->x) >= 0.0f ? true : false;
        auto ymoved = glm::abs(speed->y) >= 0.0f ? true : false;
        auto zmoved = glm::abs(speed->z) >= 0.0f ? true : false;
        dirtyFlag = xmoved || ymoved ? true : false;

    };

    for (unsigned int i = 0; i < sz; ++i) kinematic(i);

}
