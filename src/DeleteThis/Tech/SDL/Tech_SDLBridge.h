#ifndef __TECH_SDLBRIDGE_H__
#define __TECH_SDLBRIDGE_H__

#include "Tech/G/Tech.h"
#include "Texture2D_SDLBridge.h"
#include "Event/event.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <iostream>
#include <vector>

static const std::string TAG{"Tech_SDLBridge"};
namespace GTech{
    class Tech_SDLBridge : public GTech::TechnologyLibraryInterface{

        
    private:

        unsigned int m_initFlags{SDL_INIT_EVERYTHING};
        unsigned int m_rendererFlags{SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE};
        SDL_Window* pWindow{nullptr};
        SDL_Renderer* pRenderer{nullptr};
        SDL_Rect pWindowRect;
        std::vector<SDL_Joystick*> m_pjoysticks;

        SDL_Texture* GetTextureFromTexture2DRawPtr(const GTech::Texture2D *pt2d){

            auto p_sdltexture2d = dynamic_cast<const Texture2D_SDL*>(pt2d);
            if (p_sdltexture2d) return p_sdltexture2d->Get();
            return nullptr;

        }
        SDL_Texture* GetTextureFromTexture2DSmartPtr(const GTech::Texture &rspt2d){

            GTech::Texture2D* p_gtech2dtexture2d = rspt2d.get();
            if (!p_gtech2dtexture2d) return nullptr;

            auto p_sdltexture2d = dynamic_cast<Texture2D_SDL*>(p_gtech2dtexture2d);

            SDL_Texture* pSDLtexture = p_sdltexture2d->Get();
            return pSDLtexture;

        }
        SDL_Rect GetRectFromRectangle2D(const GTech::Rectangle2D& rg){
            SDL_Rect rs{rg.winPos.x, rg.winPos.y, rg.winSz.w, rg.winSz.h};
            return rs;
        }
        SDL_Point GetPointFromPoint2D(const GTech::Point2D& pg){
            SDL_Point ps;
            ps.x = pg.x;
            ps.y = pg.y;
            return ps;
        }
        bool RectIsNull(SDL_Rect &r){
            return r.w == 0 || r.h == 0;
        }

        
        /**
         * @brief      This function should be used internally ONLY by the UpdateEvents function. 
         *
         * @param      rEv  Polled event from SDL.
         * 
         */
        void DispatchKeyboardEvents(SDL_Event& rEv);


    public:
        Tech_SDLBridge(){}
        ~Tech_SDLBridge() override {};
        int Init() override;
        
        void GetWindowSize(GTech::Texture2DSize& rSz) override {

            rSz.w = pWindowRect.w;
            rSz.h = pWindowRect.h;

        };

        int CreateWindow(GTech::WindowConfiguration winConfig, unsigned int uiFlags) override;
        int CreateRenderer() override {
            pRenderer = SDL_CreateRenderer(pWindow, -1, m_rendererFlags);
            if (!pRenderer){
                std::cerr << "SDL_CreatRenderer failed. \n";
                return GTech::GTECH_ERROR;
            }
            return GTech::GTECH_OK;
        }

        int Finish() override {

            if (pRenderer) SDL_DestroyRenderer(pRenderer);
            if (pWindow) SDL_DestroyWindow(pWindow);
            SDL_Quit();
            return GTech::GTECH_OK;
        }
        GTech::Texture CreateTextureWithSize(const GTech::Texture2DSize &rSize) override {

            GTech::Texture pTexture(nullptr);

            SDL_Texture* pSDLTexture = SDL_CreateTexture(
                    pRenderer,
                    SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET,
                    rSize.w,
                    rSize.h
            );
            if (!pSDLTexture){
                std::cerr << "Tech_SDLBridge: Couldn't create a texture....\n";
                return pTexture;
            }
            pTexture.reset(new Texture2D_SDL(pSDLTexture));
            return pTexture;

        }
        GTech::Texture LoadTexture(std::string resourceName) override{

            GTech::Texture pTexture(nullptr);
            if (!pRenderer){
                std::cerr << "Tech_SDLBridge: Trying to load a texture without a renderer present.\n";
                return pTexture;
            }

            std::string localPath = std::string(RES_DIR) + resourceName;
            SDL_Surface* pImageSurface = IMG_Load(localPath.c_str());
            if (!pImageSurface){
                std::cerr << "Tech_SDLBridge: Error loading surface from: " << localPath << "\n";
                return pTexture;
            }

            SDL_Texture* pSDLTexture = SDL_CreateTextureFromSurface(pRenderer, pImageSurface);
            SDL_FreeSurface(pImageSurface);
            if (!pSDLTexture){
                std::cerr << "Tech_SDLBridge: Couldn't create a texture... \n";
                return pTexture;
            }

            pTexture.reset(new Texture2D_SDL(pSDLTexture));
            return pTexture;

        }



        int RenderTextureEx(SDL_Texture* pSDLTexture, GTech::Rectangle2D dstRect, GTech::Rectangle2D srcRect, const double angle_deg, GTech::Point2D point, GTech::FlipType flip) {

            SDL_Rect src = GetRectFromRectangle2D(srcRect);
            SDL_Rect* pSrc = RectIsNull(src) ? nullptr : &src;

            SDL_Rect dst = GetRectFromRectangle2D(dstRect);
            SDL_Rect* pDst = RectIsNull(dst) ? nullptr : &dst;

            SDL_Point p = GetPointFromPoint2D(point);
            SDL_RendererFlip f = SDL_FLIP_NONE;

            SDL_Point sdlPoint = GetPointFromPoint2D(point);
            SDL_Point* pSDLPoint = &sdlPoint;

            if (flip == GTech::FlipType::FLIP_HORIZONTAL){

                f = SDL_FLIP_HORIZONTAL;

            } else if (flip == GTech::FlipType::FLIP_VERTICAL) {

                f = SDL_FLIP_VERTICAL;

            } else if (flip == GTech::FlipType::FLIP_HORIZONTAL_AND_VERTICAL) {

                f = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL);

            }

            SDL_RenderCopyEx(pRenderer, pSDLTexture, pSrc, pDst, angle_deg, pSDLPoint, f);
            return GTech::GTECH_OK;

        }

        int RenderTextureEx(GTech::Texture2D* pTxtr, GTech::Rectangle2D dstRect, GTech::Rectangle2D srcRect, const double angle_deg, GTech::Point2D point, GTech::FlipType flip) override{

            SDL_Texture* pSDLTexture = GetTextureFromTexture2DRawPtr(pTxtr);
            if (!pSDLTexture) return GTech::GTECH_ERROR;
            return RenderTextureEx(pSDLTexture, dstRect, srcRect, angle_deg, point, flip);
        }
        int RenderTextureEx(GTech::Texture spTxtr, GTech::Rectangle2D dstRect, GTech::Rectangle2D srcRect, const double angle_deg, GTech::Point2D point, GTech::FlipType flip) override{

            SDL_Texture* pSDLTexture = GetTextureFromTexture2DSmartPtr(spTxtr);
            if (!pSDLTexture) return GTech::GTECH_ERROR;
            return RenderTextureEx(pSDLTexture, dstRect, srcRect, angle_deg, point, flip);

        }

        int RenderTexture(GTech::Texture spTxtr, GTech::Rectangle2D dstRect, GTech::Rectangle2D srcRect) override
        {
            SDL_Texture* pSDLTexture = GetTextureFromTexture2DSmartPtr(spTxtr);
            if (!pSDLTexture) return GTech::GTECH_ERROR;

            SDL_Rect src = GetRectFromRectangle2D(srcRect);
            SDL_Rect dst = GetRectFromRectangle2D(dstRect);

            SDL_RenderCopy(pRenderer, pSDLTexture, RectIsNull(src) ? nullptr : &src, RectIsNull(dst) ? nullptr : &dst);
            return GTech::GTECH_OK;
        }


        void UpdateEvents() override;
        
        virtual void RegisterKeyboardEvent(const KBEvent& rKBEvent, const KBKey& rKBKey, std::function<void(const KBEvent&, const KBKey&)>) override;
        virtual void RegisterKeyboardEvent(const KBEvent& rKBEvent, std::vector<const KBKey>, std::function<void(const KBEvent&, const KBKey&)>) override;


        unsigned long GetJoysticksCount() override {
            return m_pjoysticks.size();
        }

        void Assert(bool && exp) override {
            SDL_assert(exp);
        }


        /**** SDL SPECIFIC ****/
        /* The following functions are SDL specific. Should be used ONLY in case. */
        void SetSDLInitFlags(unsigned int uiFlags) {
            m_initFlags = uiFlags;
        }
        void SetSDLRenderFlags(unsigned int uiFlags) {
            m_rendererFlags = uiFlags;
        }
        int DetectJoysticks ( ) {

            auto sdl_njoysticks = SDL_NumJoysticks();
            if (sdl_njoysticks <= 0) {
                std::cout << TAG << ": No Joyticks attached." << std::endl;
                return GTech::GTECH_ERROR;
            } else {
                std::cout << TAG << ": Found " << sdl_njoysticks << " attached." << std::endl;
            }


            for ( auto i = 0; i < sdl_njoysticks; ++i ) {
                auto pjoystick = SDL_JoystickOpen(i);
                if (pjoystick) {
                    std::cout << TAG << " Found a valid joystick, named: " << SDL_JoystickNameForIndex(i) << std::endl;
                    std::cout << TAG << "\tAxes: " << SDL_JoystickNumAxes(pjoystick) << " - Buttons: " << SDL_JoystickNumButtons(pjoystick) << " - Balls: " << SDL_JoystickNumBalls(pjoystick) << std::endl;
                    m_pjoysticks.push_back(pjoystick);
                } else {
                    m_pjoysticks.push_back(nullptr);
                    std::cout << TAG << " Couldn't retrieve a joystick." << std::endl;
                }
            }

            return GTech::GTECH_OK;
        }
        int InitImageLoading() {
            auto imageFlags = 0;

    #ifdef PNG_FORMAT_SHOULD_BE_USED
            imageFlags |= IMG_INIT_PNG;
    #endif
    #ifdef JPG_FORMAT_SHOULD_BE_USED
            imageFlags |= IMG_INIT_JPG;
    #endif
    #ifdef TIF_FORMAT_SHOULD_BE_USED
            imageFlags |= IMG_INIT_TIF;
    #endif
            //Put your own bmp image here
            const SDL_version* pLinkedVersion = IMG_Linked_Version();
            char M = pLinkedVersion->major; M += 0x30;
            char m = pLinkedVersion->minor; m += 0x30;
            char p = pLinkedVersion->patch; p += 0x30;

            std::cout << "SDL_Image Version: " << M << "." << m << "." << p << "\n";
            imageFlags = IMG_Init(imageFlags);
            return imageFlags ? GTech::GTECH_OK : GTech::GTECH_ERROR;
        }
        SDL_Window* GetWindow(){
            return pWindow;
        }
        SDL_Renderer* GetRenderer(){
            return pRenderer;
        }
    };    
}

#endif /*__TECH_SDLBRIDGE_H__*/