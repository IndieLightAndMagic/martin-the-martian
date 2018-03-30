#include "Tech.h"
#include "Tech_SDLBridge.h"


GTech::GTPTech2D GTech::Tech2DFactory::StartTechInstance(GTech::TechDriver tech2D) {
    if ( tech2D == GTech::TechDriver::SDL){
        auto ptr = std::make_shared<Tech_SDLBridge>();
        return ptr;
    }
    return nullptr;
}

int GTech::Tech2D::DestroyTexture(GTPTexture2D& pTexture){

    //Release the texture
    pTexture.reset();
    return GTech::GTECH_OK;

}
