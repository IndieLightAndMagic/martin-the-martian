#include "rendering.h"

using namespace ECS;
std::vector<unsigned int>               RenderingSystem::ids{};
std::vector<SDL_Texture*>          		RenderingSystem::textures{};
std::vector<glm::ivec2>                 RenderingSystem::textureSizes{};
std::vector<glm::vec3*>	                RenderingSystem::positions{};
SDL_Texture* 							RenderingSystem::pScreen = nullptr;
