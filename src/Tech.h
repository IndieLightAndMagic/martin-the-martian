#ifndef __TECH_H__
#define __TECH_H__
namespace GTech2D{

    template <typename T>
    struct _2d{
        T x, y;
    };
    alias WindowSize =  _2d<int>;
    alias WindowPosition = _2d<int>;

    struct Rectangle2D{
        WindowSize winSz;
        WindowPosition winPos;
    };
    struct WindowConfiguration{
        std::string title;
        const Rectangle2D windowRectangle;
    };
    class enum TechDriver{SDL, SDL_OPENGL, ALLEGRO, ALLEGRO_OPENGL, GLFW_OPENGL};
    class Tech2D {
    public:
        Tech2D():{};
        static void CreateWindow(std::string windowTitle, WindowConfiguration winConfig, int flags) = 0;
    };
    class Tech2DFactory{

        Tech2D* StartTechInstance(Tech2D tech2D = TechDriver::SDL);
    };


}

#endif /*__TECH_H__*/