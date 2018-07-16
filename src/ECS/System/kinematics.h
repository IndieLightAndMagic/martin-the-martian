#ifndef __KINEMATICS_H__
#define __KINEMATICS_H__

#include <timer.h>
#include <ECS/Entity/entitymanager.h>
#include <ECS/Component/componentmanager.h>
#include <ECS/Component/entityinformationcomponent.h>

#include <glm/vec3.hpp>
#include <set>

//Constant aceleration kinematic model.

namespace ECS {

	class KinematicsSystem {


		static std::vector<unsigned int>	ids;
        static std::vector<glm::vec3*>		positions;
		static std::vector<glm::vec3*>		speeds;
		static std::vector<glm::vec3*>		accelerations;
        static std::vector<bool*>           positionDirtyFlags;
        static glm::vec3**                  positions_;
        static glm::vec3**                  speeds_;
        static glm::vec3**                  accelerations_;

        static bool**                       positionDirtyFlags_;
    public:
            static void InitKinematicsSystem();
            static unsigned int SubscribeEntity(unsigned int entityId);
            static void UpdateKinematicsSystem();

	};
}
#endif