#include "Tech.h"
#include "Tech_SDLBridge.h"


GTech2D::Tech2D* GTech2D::Tech2DFactory::StartTechInstance_(GTech2D::TechDriver tech2D) {

    if (tech2D == GTech2D::TechDriver::SDL) {
        auto ptr = new Tech_SDLBridge();
        return ptr;
    }
    return nullptr;

}
GTech2D::UPTech2D GTech2D::Tech2DFactory::StartTechInstance(GTech2D::TechDriver tech2D) {
    return std::make_unique<Tech2D>(StartTechInstance_(tech2D));
}

int GTech2D::Tech2D::DestroyTexture(std::unique_ptr<GTech2D::Texture2D>& pTexture){

    pTexture.reset(nullptr);
    return GTech2D::GTECH_OK;

}
