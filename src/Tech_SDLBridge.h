#ifndef __SDLBRIDGE_H__
#define __SDLBRIDGE_H__

#include "Tech.h"
#include "Texture2D_SDLBridge.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <iostream>

class Tech_SDLBridge : public GTech2D::Tech2D{
private:

    unsigned int m_initFlags{SDL_INIT_EVERYTHING};
    unsigned int m_rendererFlags{SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE};
    SDL_Window* pWindow{nullptr};
    SDL_Renderer* pRenderer{nullptr};

    SDL_Texture* GetTextureFromTexture2DPtr(std::unique_ptr<GTech2D::Texture2D>& spt2d){

        GTech2D::Texture2D* p_gtech2dtexture2d = spt2d.get();
        if (!p_gtech2dtexture2d) return nullptr;

        auto p_sdltexture2d = static_cast<Texture2D_SDL*>(p_gtech2dtexture2d);

        SDL_Texture* pSDLtexture = p_sdltexture2d->Get();
        return pSDLtexture;

    }
    SDL_Rect GetRectFromRectangle2D(GTech2D::Rectangle2D rg){
        SDL_Rect rs;
        rs.x = rg.winPos.x;
        rs.y = rg.winPos.y;
        rs.w = rg.winSz.w;
        rs.h = rg.winSz.h;

        return rs;
    }
    SDL_Point GetPointFromPoint2D(GTech2D::Point2D pg){
        SDL_Point ps;
        ps.x = pg.x;
        ps.y = pg.y;
        return ps;
    }
    bool RectIsNull(SDL_Rect &r){
        return r.w == 0 || r.h == 0;
    }

public:
    Tech_SDLBridge(){}
    ~Tech_SDLBridge() override {};
    int Init() override {
        if (SDL_Init(m_initFlags) != 0){
            std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
            return GTech2D::GTECH_ERROR;
        }
        return GTech2D::GTECH_OK;
    }
    int CreateWindow(GTech2D::WindowConfiguration winConfig, unsigned int uiFlags) override {

        pWindow = SDL_CreateWindow(
                winConfig.title.c_str(),
                winConfig.windowRectangle.winPos.x,
                winConfig.windowRectangle.winPos.y,
                winConfig.windowRectangle.winSz.w,
                winConfig.windowRectangle.winSz.h,
                uiFlags);

        if (!pWindow){
            std::cerr << "SDL_CreateWindow failed. \n";
            return GTech2D::GTECH_ERROR;
        }
        return GTech2D::GTECH_OK;

    }
    int CreateRenderer() override {
        pRenderer = SDL_CreateRenderer(pWindow, -1, m_rendererFlags);
        if (!pRenderer){
            std::cerr << "SDL_CreatRenderer failed. \n";
            return GTech2D::GTECH_ERROR;
        }
        return GTech2D::GTECH_OK;
    }

    int Finish() override {

        if (pRenderer) SDL_DestroyRenderer(pRenderer);
        if (pWindow) SDL_DestroyWindow(pWindow);
        SDL_Quit();
        return GTech2D::GTECH_OK;
    }
    std::unique_ptr<GTech2D::Texture2D> CreateTexture(const GTech2D::Texture2DSize& rSize) override {

        std::unique_ptr<GTech2D::Texture2D> pTexture(nullptr);

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
    std::unique_ptr<GTech2D::Texture2D> LoadTexture(std::string resourceName) override{

        std::unique_ptr<GTech2D::Texture2D> pTexture(nullptr);
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

    int SetRenderTarget(std::unique_ptr<GTech2D::Texture2D>& spTxtr) override{

        SDL_Texture* pSDLTexture = GetTextureFromTexture2DPtr(spTxtr);
        SDL_assert(SDL_SetRenderTarget(pRenderer, pSDLTexture) == 0);
        return GTech2D::GTECH_OK;
    }
    int DetachRenderTexture() override {
        SDL_assert(SDL_SetRenderTarget(pRenderer, nullptr) == 0);
        return GTech2D::GTECH_OK;
    }
    int RenderClear(void) override{
        SDL_assert(SDL_RenderClear(pRenderer) == 0);
        return GTech2D::GTECH_OK;
    }

    int RenderTextureEx(GTech2D::UPTexture2D& spTxtr, GTech2D::Rectangle2D dstRect, GTech2D::Rectangle2D srcRect, const double angle_deg, GTech2D::Point2D point, GTech2D::FlipType flip) override{

        SDL_Texture* pSDLTexture = GetTextureFromTexture2DPtr(spTxtr);
        if (!pSDLTexture) return GTech2D::GTECH_ERROR;

        SDL_Rect src = GetRectFromRectangle2D(srcRect);
        SDL_Rect* pSrc = RectIsNull(src) ? nullptr : &src;

        SDL_Rect dst = GetRectFromRectangle2D(dstRect);
        SDL_Rect* pDst = RectIsNull(dst) ? nullptr : &dst;

        SDL_Point p = GetPointFromPoint2D(point);
        SDL_RendererFlip f = SDL_FLIP_NONE;

        SDL_Point sdlPoint = GetPointFromPoint2D(point);
        SDL_Point* pSDLPoint = &sdlPoint;

        if (flip == GTech2D::FlipType::FLIP_HORIZONTAL){

            f = SDL_FLIP_HORIZONTAL;

        } else if (flip == GTech2D::FlipType::FLIP_VERTICAL) {

            f = SDL_FLIP_VERTICAL;

        } else if (flip == GTech2D::FlipType::FLIP_HORIZONTAL_AND_VERTICAL) {

            f = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL|SDL_FLIP_VERTICAL);

        }

        SDL_RenderCopyEx(pRenderer, pSDLTexture, pSrc, pDst, angle_deg, pSDLPoint, f);
        return GTech2D::GTECH_OK;

    }

    int RenderTexture(GTech2D::UPTexture2D& spTxtr, GTech2D::Rectangle2D dstRect, GTech2D::Rectangle2D srcRect) override
    {
        SDL_Texture* pSDLTexture = GetTextureFromTexture2DPtr(spTxtr);
        if (!pSDLTexture) return GTech2D::GTECH_ERROR;

        SDL_Rect src = GetRectFromRectangle2D(srcRect);
        SDL_Rect dst = GetRectFromRectangle2D(dstRect);

        SDL_RenderCopy(pRenderer, pSDLTexture, RectIsNull(src) ? nullptr : &src, RectIsNull(dst) ? nullptr : &dst);
        return GTech2D::GTECH_OK;
    }

    void UpdateScreen() override {
        SDL_RenderPresent(pRenderer);
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
        return imageFlags ? GTech2D::GTECH_OK : GTech2D::GTECH_ERROR;
    }
    SDL_Window* GetWindow(){
        return pWindow;
    }
    SDL_Renderer* GetRenderer(){
        return pRenderer;
    }
};
#endif /*__SDLBRIDGE_H__*/