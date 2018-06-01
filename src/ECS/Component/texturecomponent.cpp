
#include <ECS/Component/texturecomponent.h>

#include <SDL2/SDL.h>
#include <memory>
#include <sdlwrapper.h>

using namespace std;
using namespace ECS;
using namespace GTech;

TextureComponent_::TextureComponent_(std::string path) {
    SetTexture(path.c_str());
}

TextureComponent_::TextureComponent_(const char* path) {
    SetTexture(path);
}

SDL_Texture* TextureComponent_::GetTexture() {
    return m_pTexture;
}

void TextureComponent_::SetTexture(SDL_Texture* pt) {
    m_pTexture = pt;
}

void TextureComponent_::SetTexture(const char* path) {
    SetTexture(SDLCreateTextureFromFile(path));
}

void TextureComponent_::SetTexture(std::string path) {
    SetTexture(path.c_str());
}



