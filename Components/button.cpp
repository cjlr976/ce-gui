#include "button.h"
#include "renderer.h"
#include <SDL3/SDL.h>

Button::Button(Renderer* renderer, TTF_Font* f, const std::string& text)
{
    font = f;
    label = text;
    color = {100, 100, 200, 255}; // default button color
}

void Button::setText(const std::string& text) { label = text; }

void Button::draw(Renderer& renderer) {
    Widget::draw(renderer);
    if (!font || label.empty()) return;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Blended(font, label.c_str(), label.size(), white);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer.getNativeRenderer(), surf);
    SDL_FRect dst = {
        (float)posX + width / 2.0f - surf->w / 2.0f,
        (float)posY + height / 2.0f - surf->h / 2.0f,
        (float)surf->w, (float)surf->h
    };
    SDL_RenderTexture(renderer.getNativeRenderer(), tex, nullptr, &dst);
    SDL_DestroyTexture(tex);
    SDL_DestroySurface(surf);
}
