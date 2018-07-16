#include "component.h"

namespace ECS {

	void AnchorPointComponent_::SetAnchorPoint(glm::vec3 anchorPoint, glm::vec3 box)
	{
		SDL_assert(anchorPoint.x >= 0.0f && anchorPoint.x <= 1.0f);
		SDL_assert(anchorPoint.y >= 0.0f && anchorPoint.y <= 1.0f);
		SDL_assert(anchorPoint.z >= 0.0f && anchorPoint.z <= 1.0f);
		
		m_anchorPoint = anchorPoint;
		m_correctionVector = glm::vec3{-anchorPoint.x * box.x, -anchorPoint.y * box.y, -anchorPoint.z * box.z};
	}

}