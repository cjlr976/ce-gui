#include <SDL3/SDL.h>
#include <iostream>
#include "Renderer.h"
#include "Layout.h"
#include "Widget.h"

int main(int argc, char** argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Widget Layout Example", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    Renderer renderer(window);
    Layout layout;
    Widget widget;

    widget.setPosition(200, 150);
    widget.setSize(150, 150);
    layout.setWidget(&widget);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        renderer.clear();
        layout.draw(renderer);
        renderer.present();

        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
