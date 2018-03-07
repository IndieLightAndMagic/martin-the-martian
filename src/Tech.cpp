#include "Tech.h"
#include "Tech_SDLBridge.h"


GTech2D::GTPTech2D GTech2D::Tech2DFactory::StartTechInstance(GTech2D::TechDriver tech2D) {
    if ( tech2D == GTech2D::TechDriver::SDL){
        auto ptr = std::make_shared<Tech_SDLBridge>();
        return ptr;
    }
    return nullptr;
}

int GTech2D::Tech2D::DestroyTexture(GTPTexture2D& pTexture){

    //Release the texture
    pTexture.reset();
    return GTech2D::GTECH_OK;

}
