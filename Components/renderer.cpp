#include "Renderer.h"
#include <iostream>

Renderer::Renderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
    }
}

Renderer::~Renderer() {
    if (renderer) SDL_DestroyRenderer(renderer);
}

void Renderer::clear() {
    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255))
        SDL_RenderClear(renderer);
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}

bool Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

bool Renderer::fillRect(int x, int y, int w, int h) {
    SDL_FRect rect = { (float)x, (float)y, (float)w, (float)h };
    return SDL_RenderFillRect(renderer, &rect);
}
