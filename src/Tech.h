#ifndef __TECH_H__
#define __TECH_H__
#include <string>

namespace GTech2D{


    static const int GTECH_ERROR = 0x0;
    static const int GTECH_OK    = 0x1;

    template <typename T>
    struct _2d{
        T x, y;
    };

    using WindowSize =  _2d<int>;
    using WindowPosition = _2d<int>;
    using Texture2DSize = _2d<int>;

    struct Rectangle2D{
        WindowSize winSz;
        WindowPosition winPos;
    };
    struct WindowConfiguration{
        std::string title;
        const Rectangle2D windowRectangle;
    };
    enum class TechDriver{SDL, SDL_OPENGL, ALLEGRO, ALLEGRO_OPENGL, GLFW_OPENGL};
    struct Texture2D{
        Texture2D() = default;
        Texture2D(const Texture2D&) = default;
        Texture2D(Texture2D&&) = default;
        virtual ~Texture2D() = default;

    };
    class Tech2D {
    public:
        Tech2D(){};
        virtual ~Tech2D(){};

        virtual int CreateRenderer() = 0;
        virtual int CreateWindow(WindowConfiguration winConfig, unsigned int flags) = 0;
        virtual int DestroyTexture(std::unique_ptr<GTech2D::Texture2D>&);
        virtual int Finish() = 0;
        virtual int Init() = 0;

        virtual std::unique_ptr<GTech2D::Texture2D> LoadTexture(std::string textureFilename) = 0;
        virtual std::unique_ptr<GTech2D::Texture2D> CreateTexture(const GTech2D::Texture2DSize&) = 0;


        virtual void Assert(bool && exp) = 0;



    };
    class Tech2DFactory{
    public:
        static Tech2D* StartTechInstance( GTech2D::TechDriver tech2D = GTech2D::TechDriver::SDL );
    };


}

#endif /*__TECH_H__*/