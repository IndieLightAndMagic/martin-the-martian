#include "rendering.h"

using namespace ECS;
std::vector<unsigned int>               RenderingSystem::ids{};
std::vector<SDL_Texture*>          		RenderingSystem::textures{};
std::vector<ECS::PositionComponent*>	RenderingSystem::positions{};


