
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
const std::tuple<unsigned long, unsigned long> TextureComponent_::GetScaledSize() {
    return std::tuple<unsigned long, unsigned long>{ m_scaledSize_16W_16H>>16, m_scaledSize_16W_16H & 0xffff };
}
void TextureComponent_::SetTexture(SDL_Texture* pt) {

    m_pTexture = pt;
    auto [format, access, sz] = SDLQueryTexture(pt);

    m_pOriginalSize = sz;
    m_scaledSize_16W_16H = (m_pOriginalSize.x << 16) + m_pOriginalSize.y;
    
}

void TextureComponent_::SetTexture(const char* path) {
    SetTexture(SDLCreateTextureFromFile(path));
}

void TextureComponent_::SetTexture(std::string path) {
    SetTexture(path.c_str());
}

void TextureComponent_::SetScale(float scale) {
    SDL_assert(m_pTexture!= nullptr);

    glm::vec2 tsz{m_pOriginalSize.x*scale, m_pOriginalSize.y*scale};
    glm::ivec2 tszi{tsz.x, tsz.y};
    m_scaledSize_16W_16H = (tszi.x << 16) + tszi.y;
}

