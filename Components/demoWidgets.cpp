#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "renderer.h"
#include "widget.h"
#include "button.h"
#include "label.h"
#include "slider.h"
#include "toggle.h"

int main(int argc, char** argv) {
    // --- SDL3 Initialization ---
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!TTF_Init()) {
        std::cerr << "TTF Init failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    int winW = 800;
    int winH = 600;
    SDL_Window* window = SDL_CreateWindow("Widget Toolbox Demo", winW, winH, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Renderer renderer(window);

    // --- Load Font ---
    TTF_Font* font = TTF_OpenFont("Roboto_SemiCondensed-Black.ttf", 24);
    if (!font) std::cerr << "Failed to load font" << std::endl;

    // --- Create Widgets ---
    Button btn(&renderer, font, "Click Me");
    btn.setPosition(50, 50);
    btn.setSize(150, 50);
    btn.setOnClick([](){ std::cout << "Button clicked!\n"; });

    Label lbl(&renderer, font, "Hello, SDL3!");
    lbl.setPosition(50, 120);

    Slider slider;
    slider.setPosition(50, 200);
    slider.setSize(300, 40);
    slider.setRange(0, 100);

    ToggleSwitch toggle;
    toggle.setPosition(50, 300);
    toggle.setSize(60, 30);

    SDL_Event e;
    bool running = true;

    SDL_StartTextInput(window);

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;

            btn.handleEvent(e);
            slider.handleEvent(e);
            toggle.handleEvent(e);
        }

        // --- Rendering ---
        renderer.setDrawColor(30, 30, 30, 255); // background
        renderer.clear();

        btn.draw(renderer);
        lbl.draw(renderer);
        slider.draw(renderer);
        toggle.draw(renderer);

        renderer.present();

        SDL_Delay(16); // ~60 FPS
    }

    SDL_StopTextInput(window);

    if (font) TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
