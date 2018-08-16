#ifndef __SHIP__
#define __SHIP__

#include <set>
#include <SDL2/SDL_image.h>

#include <ECS/Component/entityinformationcomponent.h>
#include <ECS/Component/texturecomponent.h>
#include <ECS/Component/componentmanager.h>

#include <ECS/Entity/entitymanager.h>

SDL_Texture* SDLCreateTextureFromSurface(SDL_Surface* pSurface);
namespace GTech{


    class Sprite {

    public:

        static unsigned int CreateSprite(std::string path);
        static void SetPosition(unsigned int shipId, glm::vec3 position);
        static void SetAnchorPoint(unsigned int shipId, glm::vec3 anchor);
        static void SetScale(unsigned int shipId, float scale);

    };
}


#endif /* __SHIP__ */
