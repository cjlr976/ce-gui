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
    if (!renderer) return;
    // default clear with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::present() {
    if (renderer) SDL_RenderPresent(renderer);
}

bool Renderer::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (!renderer) return false;
    return SDL_SetRenderDrawColor(renderer, r, g, b, a) == 0;
}

bool Renderer::fillRect(int x, int y, int w, int h) {
    if (!renderer) return false;
    SDL_FRect rect = { (float)x, (float)y, (float)w, (float)h };
    return SDL_RenderFillRect(renderer, &rect) == 0;
}

SDL_Renderer* Renderer::getNativeRenderer() {
    return renderer;
}