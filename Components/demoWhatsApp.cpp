#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "layout.h"
#include "toggle.h"
#include "button.h"

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

    SDL_Window* window = SDL_CreateWindow("Chat UI Demo with Contact Highlights", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Renderer renderer(window);

    TTF_Font* font = TTF_OpenFont("Roboto_SemiCondensed-Black.ttf", 20);
    if (!font) std::cerr << "Failed to load font" << std::endl;

    // Textbox widget
    TextBox textbox(&renderer, font);
    auto computeTextboxPosition = [&]() {
        int tbW = static_cast<int>(windowWidth * 0.4f);
        int tbH = static_cast<int>(windowHeight * 0.06f);
        int tbX = static_cast<int>(windowWidth * 0.35f + (windowWidth * 0.6f - tbW)/2.0f);
        int tbY = static_cast<int>(windowHeight * 0.85f);
        textbox.setBox(tbX, tbY, tbW, tbH);
    };
    computeTextboxPosition();

    // Dark mode toggle
    ToggleSwitch darkModeToggle;
    darkModeToggle.setPosition(windowWidth - 80, 20);
    darkModeToggle.setSize(60, 30);
    bool darkMode = false;
    darkModeToggle.onToggle = [&](bool isDark) { darkMode = isDark; };

    SDL_StartTextInput(window);

    bool running = true;
    SDL_Event e;

    const int nContacts = 10;
    SDL_FRect contactRects[nContacts];  // contact rectangles
    SDL_Color contactColors[nContacts]; // current color of each contact

    // Initialize contact colors
    for(int i = 0; i < nContacts; ++i) {
        contactColors[i] = {230, 230, 230, 255};
    }

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;

            if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                windowWidth = e.window.data1;
                windowHeight = e.window.data2;
                computeTextboxPosition();
                darkModeToggle.setPosition(windowWidth - 80, 20);
            }

            textbox.handleEvent(e);
            darkModeToggle.handleEvent(e);

            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                for (int i = 0; i < nContacts; ++i) {
                    SDL_FRect &r = contactRects[i];
                    if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h) {
                        // Change this contact color randomly
                        contactColors[i] = {Uint8(rand() % 256), Uint8(rand() % 256), Uint8(rand() % 256), 255};
                        textbox.clear();
                        break;
                    }
                }
            }
        }

        // Background depending on dark mode
        int bgR = darkMode ? 40 : 245;
        int bgG = darkMode ? 40 : 245;
        int bgB = darkMode ? 40 : 245;
        renderer.setDrawColor(bgR, bgG, bgB, 255);
        renderer.clear();

        SDL_Renderer* native = renderer.getNativeRenderer();

        // Left panel
        SDL_FRect leftPanel = { windowWidth * 0.05f, windowHeight * 0.05f, windowWidth * 0.25f, windowHeight * 0.9f };
        int leftR = darkMode ? 60 : 180;
        int leftG = darkMode ? 60 : 220;
        int leftB = darkMode ? 60 : 190;
        renderer.setDrawColor(leftR, leftG, leftB, 255);
        SDL_RenderFillRect(native, &leftPanel);
        renderer.setDrawColor(leftR - 20, leftG - 20, leftB - 20, 255);
        SDL_RenderRect(native, &leftPanel);

        // Contacts
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
            SDL_Color c = contactColors[i];
            renderer.setDrawColor(c.r, c.g, c.b, c.a);
            SDL_RenderFillRect(native, &contact);
            renderer.setDrawColor(c.r - 50, c.g - 50, c.b - 50, 255);
            SDL_RenderRect(native, &contact);
            contactRects[i] = contact;
        }

        // Right panel
        SDL_FRect rightPanel = { windowWidth * 0.35f, windowHeight * 0.05f, windowWidth * 0.6f, windowHeight * 0.9f };
        renderer.setDrawColor(darkMode ? 60 : 180, darkMode ? 60 : 220, darkMode ? 60 : 190, 255);
        SDL_RenderFillRect(native, &rightPanel);
        renderer.setDrawColor(darkMode ? 40 : 150, darkMode ? 40 : 200, darkMode ? 40 : 200, 255);
        SDL_RenderRect(native, &rightPanel);

        // Textbox
        textbox.draw(renderer);

        // Dark mode toggle
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
