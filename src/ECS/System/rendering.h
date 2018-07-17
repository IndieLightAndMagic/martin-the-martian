#ifndef __RENDERING__ 
#define __RENDERING__ 

#include <vector>
#include <utility>
#include <algorithm>

#include <SDL2/SDL_system.h>

#include <sdlwrapper.h>
#include <ECS/Component/component.h>
#include <ECS/Component/componentmanager.h>
#include <ECS/Component/texturecomponent.h>
#include <ECS/Entity/entitymanager.h>
#include <ECS/Component/entityinformationcomponent.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using namespace GTech;

namespace ECS {
    using RenderingDataTuple = std::tuple<
            unsigned int,            ///ids
            SDL_Texture*,            ///textures
            const unsigned long*,    ///textureSizes
            void*,                   ///positions. void* because because tuple wouldn't handle &glm::vec3.
            void*,                   ///angles per axis. void* because because tuple wouldn't handle &glm::vec3.
            void*,                   ///anchor point relative.
            void*,                   ///anchor point correction.
            bool*>;                  ///isDirty

    class RenderingSystem {

        static std::vector<ECS::RenderingDataTuple> renderingData;

        ////Screen Texture && Screen Rectangle
        static SDL_Texture* pScreen;
        static SDL_Rect pScreenRect;

        ///Coordinate System: Center x to the right y up and z towards the user.
        ///Coordinate System: width > height ? W[0,1] x H[0, height/width] : W[0, width/height] x H[0,1]
        static glm::mat4x4 mtxSDLScreenCoordinates;


        public:

        static unsigned long SubscribeEntity(unsigned int entityId);
        static unsigned int DrawSprites2D();
        static void InitRenderingSystem();
        static void ShutdownRenderingSystem();

        static void UpdateRenderingSystem();

    };

}
#endif /*__RENDERING__ */
 
