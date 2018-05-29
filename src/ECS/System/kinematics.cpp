#include <ECS/System/kinematics.h>

using namespace ECS;


std::vector<unsigned int>	KinematicsSystem::ids{};
std::vector<glm::vec3*>		KinematicsSystem::positions{};
std::vector<glm::vec3*>		KinematicsSystem::speeds{};
std::vector<glm::vec3*>		KinematicsSystem::accelerations{};


