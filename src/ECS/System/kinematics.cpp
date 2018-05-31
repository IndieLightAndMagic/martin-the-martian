#include <ECS/System/kinematics.h>

using namespace ECS;


std::vector<unsigned int>	KinematicsSystem::ids{};
std::vector<glm::vec3*>		KinematicsSystem::positions{};
std::vector<glm::vec3*>		KinematicsSystem::speeds{};
std::vector<glm::vec3*>		KinematicsSystem::accelerations{};
glm::vec3**                 KinematicsSystem::positions_ = nullptr;
glm::vec3**                 KinematicsSystem::speeds_ = nullptr;
glm::vec3**                 KinematicsSystem::accelerations_ = nullptr;
uint64_t                    KinematicsSystem::dtTicks = SDL_GetPerformanceCounter();
bool                        KinematicsSystem::firstTime = true;


void KinematicsSystem::InitKinematicsSystem(){
    firstTime = true;
}

unsigned int KinematicsSystem::SubscribeEntity(unsigned int entityId){

    auto& entityManager      = ECS::EntityManager::GetInstance();
    auto& componentManager   = ECS::ComponentManager::GetInstance();
    auto componentVectors   = entityManager.GetComponentsIds(entityId);

    //CONVENTION: the first component of ANY entity is the EntityInformationComponent_
    auto entityInfo         = componentVectors[0];
    auto entityInfoRP       = componentManager.GetComponentRaw<ECS::EntityInformationComponent_>(entityInfo);

    //Kinematic Triad
    auto [posId, speedId, accelId]  = entityInfoRP->GetKinematicTupleIds();

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

    return 1;

}

void KinematicsSystem::UpdateKinematicsSystem(){

    //x(t) = 0.5a(t)**2 + vo(t) + xo
    //s(t) = a(t) + vo
    //a(t)
    if (firstTime){
        dtTicks = SDL_GetPerformanceCounter();
        firstTime = false;
        return;
    }
    dtTicks = SDL_GetPerformanceCounter() - dtTicks;
    float dtSecs = 1.0f / SDL_GetPerformanceFrequency(); //How long does it take to one tick
    dtSecs *= dtTicks;

    auto sz = ids.size();
    auto dtSecs2    = dtSecs * dtSecs;
    auto kinematic  = [&](int index){

        auto accel  = accelerations_[index];
        auto speed  = speeds_[index];
        auto& pos   = *(positions_[index]);
        glm::vec3 accelDelta(0.5 * accel->x * dtSecs2, 0.5 * accel->y * dtSecs2, 0.5 * accel->z * dtSecs2);
        glm::vec3 speedDelta(speed->x * dtSecs, speed->y * dtSecs, speed->z * dtSecs);
        pos += accelDelta + speedDelta;

    };

    for (unsigned int i = 0; i < sz; ++i) kinematic(i);

}
