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
    enum class TechDriver { SDL, SDL_OPENGL};

    class Tech2D {
        Tech2D* pTech;
        TechDriver m_driver;
    public:
        Tech2D(TechDriver driver = TechDriver::SDL):pTech(nullptr), m_driver(){};
        static void CreateWindow(std::string windowTitle, WindowConfiguration winConfig, int flags) = 0;
    };



}

#endif /*__TECH_H__*/