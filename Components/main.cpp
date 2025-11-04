#include <SDL3/SDL.h>
#include <iostream>
#include "Renderer.h"
#include "Widget.h"

void onButtonClick() {
    std::cout << "Widget clicked!\n";
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL3 Widget Click", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    Renderer renderer(window);

    Widget widget;
    widget.setPosition(300, 250);
    widget.setSize(200, 100);
    widget.setOnClick(onButtonClick);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            widget.handleEvent(e);
        }

        renderer.clear();
        widget.draw(renderer);
        renderer.present();

        SDL_Delay(16); // ~60fps
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
