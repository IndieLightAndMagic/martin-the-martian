#include "Tech/G/Tech.h"
#include "Tech/SDL/Tech_SDLBridge.h"


GTech::Tech GTech::Tech2DFactory::StartTechInstance(GTech::TechDriver tech2D) {
    
    if ( tech2D == GTech::TechDriver::SDL){
        auto ptr = std::make_shared<Tech_SDLBridge>();
        return ptr;
    }
    return nullptr;

}

int GTech::TechnologyLibraryInterface::DestroyTexture(Texture& pTexture){

    //Release the texture
    pTexture.reset();
    return GTech::GTECH_OK;

}
