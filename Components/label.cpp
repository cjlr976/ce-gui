#include "label.h"

using namespace std;

Label::Label(Renderer* r, TTF_Font* f, const std::string& text)
    : rendererPtr(r), font(f), label(text), textColor({255,255,255,255}) {}

void Label::setText(const std::string& text) { label = text; }
void Label::setColor(SDL_Color c) { textColor = c; }
void Label::draw(Renderer& renderer) {
    if (!font || label.empty()) return;

    SDL_Renderer* native = renderer.getNativeRenderer();
    if (!native) return;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, label.c_str(), label.size(), white);
    if (!surf) {
        std::cerr << "TTF_RenderText_Blended failed: " << SDL_GetError() << endl;
        return;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(native, surf);
    if (!tex) {
        std::cerr << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << std::endl;
        SDL_DestroySurface(surf);
        return;
    }

    SDL_FRect dst = {
        (float)posX,
        (float)posY,
        (float)surf->w,
        (float)surf->h
    };


    SDL_RenderTexture(native, tex, nullptr, &dst);

    SDL_DestroyTexture(tex);
    SDL_DestroySurface(surf);
}
