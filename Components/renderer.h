#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>

class Renderer {
public:
    Renderer(SDL_Window* window);
    ~Renderer();

    void clear();
    void present();

    bool setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    bool fillRect(int x, int y, int w, int h);

    // expose the underlying SDL_Renderer for widgets that need texture/text rendering
    SDL_Renderer* getNativeRenderer();

private:
    SDL_Renderer* renderer;
};

#endif