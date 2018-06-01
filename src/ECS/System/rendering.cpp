#include "rendering.h"

using namespace ECS;
std::vector<unsigned int>               RenderingSystem::ids{};
std::vector<SDL_Texture*>          		RenderingSystem::textures{};
std::vector<glm::ivec2>                 RenderingSystem::textureSizes{};
std::vector<glm::vec3*>	                RenderingSystem::positions{};
SDL_Texture* 							RenderingSystem::pScreen = nullptr;
SDL_Rect                                RenderingSystem::pScreenRect{0, 0, 0, 0};
glm::mat4x4                             RenderingSystem::mtxSDLScreenCoordinates({1.0f, 0.0f, 0.0f, 0},{0.0f, 1.0f, 0.0f, 0.0f},{0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0});
