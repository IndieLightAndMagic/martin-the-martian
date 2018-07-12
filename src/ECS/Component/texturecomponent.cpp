
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
const unsigned long * TextureComponent_::GetSize() {
    return &m_sz;
}
void TextureComponent_::SetTexture(SDL_Texture* pt) {

    m_pTexture = pt;
    auto [format, access, sz] = SDLQueryTexture(pt);
    m_sz = (sz.x << 16) + sz.y;

}

void TextureComponent_::SetTexture(const char* path) {
    SetTexture(SDLCreateTextureFromFile(path));
}

void TextureComponent_::SetTexture(std::string path) {
    SetTexture(path.c_str());
}

void TextureComponent_::SetScale(float scale) {

    SDL_assert(m_pTexture!= nullptr);

    auto [format, access, sz] = SDLQueryTexture(m_pTexture);
    glm::vec2 tsz{sz.x*scale, sz.y*scale};
    glm::ivec2 tszi{tsz.x, tsz.y};
    m_sz = (tszi.x << 16) + tszi.y;


}

