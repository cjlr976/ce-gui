#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "layout.h"
#include "toggle.h"

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

    SDL_Window* window = SDL_CreateWindow("Chat UI Demo with Dark Mode", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Renderer renderer(window);

    TTF_Font* font = TTF_OpenFont("Roboto_SemiCondensed-Black.ttf", 20);
    if (!font) std::cerr << "Failed to load font" << std::endl;
    SDL_FRect rightPanel = { windowWidth * 0.35f, windowHeight * 0.05f, windowWidth * 0.6f, windowHeight * 0.9f };

    // Textbox widget
    TextBox textbox(&renderer, font);

    auto computeTextboxPosition = [&]() {
        int tbW = static_cast<int>(rightPanel.w * 0.9f);  // 90% width of right panel
        int tbH = static_cast<int>(windowHeight * 0.06f); // same height
        int tbX = static_cast<int>(rightPanel.x + (rightPanel.w - tbW) / 2); // center horizontally
        int tbY = static_cast<int>(rightPanel.y + rightPanel.h - tbH - 10);  // 10 px margin from bottom
        textbox.setBox(tbX, tbY, tbW, tbH);
    };

    computeTextboxPosition();

    // Toggle widget for Dark Mode
    ToggleSwitch darkModeToggle;
    darkModeToggle.setPosition(windowWidth - 80, 20);
    darkModeToggle.setSize(60, 30);

    bool darkMode = false;

    // Toggle callback for immediate effect
    darkModeToggle.onToggle = [&](bool isDark){
        darkMode = isDark;
    };

    SDL_StartTextInput(window);

    bool running = true;
    SDL_Event e;

    const int nContacts = 8;
    SDL_FRect sideRects[nContacts]; // contact rectangles

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;

            if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                windowWidth = e.window.data1;
                windowHeight = e.window.data2;
                computeTextboxPosition();

                // Move toggle to top-right corner
                darkModeToggle.setPosition(windowWidth - 80, 20);
            }

            textbox.handleEvent(e);
            darkModeToggle.handleEvent(e);

            // Click to clear textbox when contact clicked
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                for (int i = 0; i < nContacts; ++i) {
                    SDL_FRect &r = sideRects[i];
                    if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h) {
                        textbox.clear();
                        break;
                    }
                }
            }
        }

        // --- Colors depending on dark mode ---
        int bgR = darkMode ? 40 : 245;
        int bgG = darkMode ? 40 : 245;
        int bgB = darkMode ? 40 : 245;

        // Panel color (#b4d4ba)
        int panelR = darkMode ? 60 : 180;
        int panelG = darkMode ? 60 : 212;
        int panelB = darkMode ? 60 : 186;

        int contactR = darkMode ? 80 : 230;
        int contactG = darkMode ? 80 : 230;
        int contactB = darkMode ? 80 : 230;

        renderer.setDrawColor(bgR, bgG, bgB, 255);
        renderer.clear();

        SDL_Renderer* native = renderer.getNativeRenderer();

        // Left panel
        SDL_FRect leftPanel = { windowWidth * 0.05f, windowHeight * 0.05f, windowWidth * 0.25f, windowHeight * 0.9f };
        renderer.setDrawColor(panelR, panelG, panelB, 255);
        SDL_RenderFillRect(native, &leftPanel);
        renderer.setDrawColor(panelR - 20, panelG - 20, panelB - 20, 255);
        SDL_RenderRect(native, &leftPanel);

        // Contacts inside left panel
        float contactW = leftPanel.w * 0.9f;
        float contactH = leftPanel.h * 0.1f;
        float spacing = (leftPanel.h - nContacts * contactH) / (nContacts + 1);

        for (int i = 0; i < nContacts; ++i) {
            SDL_FRect contact = {
                leftPanel.x + leftPanel.w * 0.05f,
                leftPanel.y + spacing * (i + 1) + contactH * i,
                contactW,
                contactH
            };
            renderer.setDrawColor(contactR, contactG, contactB, 255);
            SDL_RenderFillRect(native, &contact);
            renderer.setDrawColor(contactR - 50, contactG - 50, contactB - 50, 255);
            SDL_RenderRect(native, &contact);

            sideRects[i] = contact;
        }

        // Right panel (chat)
        SDL_FRect rightPanel = { windowWidth * 0.35f, windowHeight * 0.05f, windowWidth * 0.6f, windowHeight * 0.9f };
        renderer.setDrawColor(panelR, panelG, panelB, 255);
        SDL_RenderFillRect(native, &rightPanel);
        renderer.setDrawColor(panelR - 20, panelG - 20, panelB - 20, 255);
        SDL_RenderRect(native, &rightPanel);

        // Draw textbox at bottom center of right panel
        textbox.draw(renderer);

        // Draw dark mode toggle
        darkModeToggle.draw(renderer);

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
