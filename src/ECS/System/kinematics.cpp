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


