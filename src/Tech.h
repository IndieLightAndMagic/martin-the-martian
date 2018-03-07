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

    template <typename T>
    struct _2d_size{
        T w, h;
    };

    template <typename T>
    struct _3d{
        T x, y, z;
    };
    template <typename T>
    struct _color{
        T r,g,b,a;
    };

    using WindowSize =  _2d_size<int>;
    using WindowPosition = _2d<int>;
    using Texture2DSize = _2d_size<int>;

    using Point2D = _2d<int>;

    using Vector2Dd = _2d<double>;
    using Vector2Df = _2d<float>;
    using Vector2Di = _2d<int>;

    using Point3D = _3d<int>;

    using Vector3Dd = _3d<double>;
    using Vector3Df = _3d<float>;
    using Vector3Di = _3d<int>;

    struct Rectangle2D{
        WindowSize winSz;
        WindowPosition winPos;
        Rectangle2D():winSz({0, 0}), winPos({0, 0}){}
        Rectangle2D(const Rectangle2D& r):winSz(r.winSz),winPos(r.winPos){}
        Rectangle2D(int x, int y, int w, int h):winSz({w,h}), winPos({x,y}){}

    };
    static Rectangle2D Zero = Rectangle2D();
    struct WindowConfiguration{
        std::string title;
        const Rectangle2D windowRectangle;
    };

    enum class FlipType{FLIP_NO, FLIP_HORIZONTAL, FLIP_VERTICAL, FLIP_HORIZONTAL_AND_VERTICAL};
    enum class TechDriver{SDL, SDL_OPENGL, ALLEGRO, ALLEGRO_OPENGL, GLFW_OPENGL};
    struct Texture2D{
        Texture2D() = default;
        Texture2D(const Texture2D&) = default;
        Texture2D(Texture2D&&) = default;
        virtual ~Texture2D() = default;
        virtual int GetSize(GTech2D::Texture2DSize&){return GTECH_ERROR;}

        Point2D Center(){

            Texture2DSize sz;
            GetSize(sz);
            return Point2D{sz.w >> 1, sz.h >> 1};
        }

    };
    using GTPTexture2D = std::shared_ptr<GTech2D::Texture2D>;

    class Tech2D {
    public:
        Tech2D(){};
        virtual ~Tech2D(){};



        virtual int CreateRenderer() = 0;
        virtual int CreateWindow(WindowConfiguration winConfig, unsigned int flags) = 0;

        virtual int DestroyTexture(GTPTexture2D&);

        virtual int Finish() = 0;
        virtual int Init() = 0;

        virtual GTech2D::GTPTexture2D LoadTexture(std::string textureFilename) = 0;
        virtual GTech2D::GTPTexture2D CreateTextureWithSize(const GTech2D::Texture2DSize&) = 0;

        virtual int RenderTextureEx(GTech2D::GTPTexture2D, GTech2D::Rectangle2D dstRect, GTech2D::Rectangle2D srcRect, const double angle_deg, GTech2D::Point2D point , GTech2D::FlipType flip = GTech2D::FlipType::FLIP_NO) = 0;
        virtual int RenderTexture(GTech2D::GTPTexture2D, GTech2D::Rectangle2D dstRect = GTech2D::Rectangle2D(), GTech2D::Rectangle2D srcRect = GTech2D::Rectangle2D()) = 0;
        virtual int DetachRenderTexture() = 0;
        virtual int SetRenderTarget(GTech2D::GTPTexture2D) = 0;
        virtual int RenderClear() = 0;

        virtual void UpdateScreen() = 0;
        virtual void Assert(bool && exp) = 0;

    };
    using GTPTech2D = std::shared_ptr<Tech2D>;
    class Tech2DFactory{
    public:
        static GTPTech2D StartTechInstance(GTech2D::TechDriver tech2D = GTech2D::TechDriver::SDL);
    };

}

#endif /*__TECH_H__*/