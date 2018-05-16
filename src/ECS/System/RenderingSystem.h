#ifndef __RENDERINGSYSTEM_H__
#define __RENDERINGSYSTEM_H__

#include <set>
#include <vector>

#include <ECS/Component/componentmanager.h>
namespace ECS{

    class RenderingSystem {
        static std::vector<unsigned int>            ids;
        static std::vector<GTech::Texture2D*>       textures;
        static std::vector<GTech::Vector2Dd*>       anchors;
        static std::vector<double*>                 scales;
        static std::vector<ECS::PositionComponent*> positions;
        static unsigned int*                        ids_;
        static GTech::Texture2D**                   textures_;
        static GTech::Vector2Dd**                   anchors_;
        static double**                             scales_;
        static ECS::PositionComponent**             positions_;


        //Screen Context
        static GTech::Texture pScreen;
    public:
        static unsigned int SubscribeEntity(unsigned int id);
        static unsigned int DrawSprites(const GTech::Tech& rptech);
        static int InitRenderingSystem(const GTech::Tech& rpTech);
        static bool ShutdownRenderingSystem(const GTech::Tech& rpTech);
        static void UpdateRenderingSystem(const GTech::Tech& rpTech);
    };

}


#endif /*__RENDERINGSYSTEM_H__ */