#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "layout.h"

int windowWidth = 800;
int windowHeight = 600;

int main(int argc, char** argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!TTF_Init()) {
        std::cerr << "TTF Init failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Chat UI Demo", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Renderer renderer(window);

    TTF_Font* font = TTF_OpenFont("Roboto_SemiCondensed-Black.ttf", 20);
    if (!font) std::cerr << "Failed to load font" << std::endl;

    TextBox textbox(&renderer, font);

    auto computeTextboxPosition = [&]() {
        SDL_FRect rightPanel = { windowWidth * 0.35f, windowHeight * 0.05f, windowWidth * 0.6f, windowHeight * 0.9f };

        int tbW = static_cast<int>(rightPanel.w * 0.9f);   // width relative to right panel
        int tbH = static_cast<int>(windowHeight * 0.12f);  // height of textbox
        int tbX = static_cast<int>(rightPanel.x + (rightPanel.w - tbW) / 2.0f); // centered horizontally
        int tbY = static_cast<int>(rightPanel.y + rightPanel.h - tbH - 10);     // near bottom with 10px margin

        textbox.setBox(tbX, tbY, tbW, tbH);
    };


    computeTextboxPosition();

    SDL_StartTextInput(window);

    bool running = true;
    SDL_Event e;

    SDL_FRect sideRects[5]; // store rectangles for contacts

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;

            if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                windowWidth = e.window.data1;
                windowHeight = e.window.data2;
                computeTextboxPosition();
            }

            textbox.handleEvent(e);

            // Click to clear textbox
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                for (int i = 0; i < 5; ++i) {
                    SDL_FRect &r = sideRects[i];
                    if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h) {
                        textbox.clear();
                        break;
                    }
                }
            }
        }

        // --- Light background ---
        renderer.setDrawColor(245, 245, 245, 255);
        renderer.clear();

        SDL_Renderer* native = renderer.getNativeRenderer();

        // Big left panel
        SDL_FRect leftPanel = { windowWidth * 0.05f, windowHeight * 0.05f, windowWidth * 0.25f, windowHeight * 0.9f };
        renderer.setDrawColor(227, 204, 197, 255); // Beige
        SDL_RenderFillRect(native, &leftPanel);
        renderer.setDrawColor(200, 200, 200, 255); // border
        SDL_RenderRect(native, &leftPanel);

        // Smaller "contact" rectangles inside left panel
        int nContacts = 10;
        float contactW = leftPanel.w * 0.9f;
        float contactH = leftPanel.h * 0.08f;
        float spacing = (leftPanel.h - nContacts * contactH) / (nContacts + 1);

        for (int i = 0; i < nContacts; ++i) {
            SDL_FRect contact = {
                leftPanel.x + leftPanel.w * 0.05f,
                leftPanel.y + spacing * (i + 1) + contactH * i,
                contactW,
                contactH
            };
            renderer.setDrawColor(230, 230, 230, 255);
            SDL_RenderFillRect(native, &contact);
            renderer.setDrawColor(180, 180, 180, 255);
            SDL_RenderRect(native, &contact);

            sideRects[i] = contact;
        }

        // Right panel (chat + textbox)
        SDL_FRect rightPanel = { windowWidth * 0.35f, windowHeight * 0.05f, windowWidth * 0.6f, windowHeight * 0.9f };
        renderer.setDrawColor(227, 204, 197, 255);
        SDL_RenderFillRect(native, &rightPanel);
        renderer.setDrawColor(200, 200, 200, 255);
        SDL_RenderRect(native, &rightPanel);

        // Draw the textbox at bottom
        textbox.draw(renderer);
        

        renderer.present();
        SDL_Delay(16);
    }

    SDL_StopTextInput(window);
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
