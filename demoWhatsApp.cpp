#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cmath>
#include <iostream>
#include "layout.h"
#include "toggle.h"
#include "button.h"
#include "textbox.h"

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;

// --------- Rounded rectangle helper function ---------
void fillRoundedRect(SDL_Renderer* renderer, SDL_FRect rect, float radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Fill center rectangle
    SDL_FRect centerRect = { rect.x + radius, rect.y, rect.w - 2*radius, rect.h };
    SDL_RenderFillRect(renderer, &centerRect);

    // Fill side rectangles
    SDL_FRect leftRect   = { rect.x, rect.y + radius, radius, rect.h - 2*radius };
    SDL_FRect rightRect  = { rect.x + rect.w - radius, rect.y + radius, radius, rect.h - 2*radius };
    SDL_RenderFillRect(renderer, &leftRect);
    SDL_RenderFillRect(renderer, &rightRect);

    // Fill corners with small rectangles approximating rounded corners
    int steps = 8;
    for(int i = 0; i <= steps; ++i) {
        float t = i / float(steps);
        float cornerOffset = radius * (1 - std::cos(t * M_PI_2));
        float yOffset = radius * std::sin(t * M_PI_2);

        // Top-left
        SDL_FRect tl = { rect.x + radius - cornerOffset, rect.y + radius - yOffset, 1.0f, 1.0f };
        SDL_RenderFillRect(renderer, &tl);

        // Top-right
        SDL_FRect tr = { rect.x + rect.w - radius + cornerOffset, rect.y + radius - yOffset, 1.0f, 1.0f };
        SDL_RenderFillRect(renderer, &tr);

        // Bottom-right
        SDL_FRect br = { rect.x + rect.w - radius + cornerOffset, rect.y + rect.h - radius + yOffset, 1.0f, 1.0f };
        SDL_RenderFillRect(renderer, &br);

        // Bottom-left
        SDL_FRect bl = { rect.x + radius - cornerOffset, rect.y + rect.h - radius + yOffset, 1.0f, 1.0f };
        SDL_RenderFillRect(renderer, &bl);
    }
}

// ------------------------------------------------------

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
        int tbW = static_cast<int>(windowWidth * 0.4f);
        int tbH = static_cast<int>(windowHeight * 0.06f);
        int tbX = static_cast<int>(windowWidth * 0.35f + (windowWidth * 0.6f - tbW)/2.0f);
        int tbY = static_cast<int>(windowHeight * 0.85f);
        textbox.setBox(tbX, tbY, tbW, tbH);
    };
    computeTextboxPosition();

    ToggleSwitch darkModeToggle;
    darkModeToggle.setPosition(windowWidth - 80, 20);
    darkModeToggle.setSize(60, 30);
    bool darkMode = false;
    darkModeToggle.onToggle = [&](bool isDark) { darkMode = isDark; };

    SDL_StartTextInput(window);

    bool running = true;
    SDL_Event e;

    const int nContacts = 10;
    SDL_FRect contactRects[nContacts];
    SDL_Color contactColors[nContacts];
    for (int i = 0; i < nContacts; ++i) contactColors[i] = {230, 230, 230, 255};

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
                        contactColors[i] = {Uint8(rand() % 256), Uint8(rand() % 256), Uint8(rand() % 256), 255};
                        textbox.clear();
                        break;
                    }
                }
            }
        }

        // Background
        SDL_Color bgColor = darkMode ? SDL_Color{40, 40, 40, 255} : SDL_Color{255, 255, 255, 255}; // white background
        renderer.setDrawColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        renderer.clear();

        SDL_Renderer* native = renderer.getNativeRenderer();

        // Left panel
        SDL_FRect leftPanel = { windowWidth * 0.05f, windowHeight * 0.05f, windowWidth * 0.25f, windowHeight * 0.9f };
        SDL_Color leftColor = darkMode ? SDL_Color{60,60,60,255} : SDL_Color{180,220,190,255};
        fillRoundedRect(native, leftPanel, 15.0f, leftColor);

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
            fillRoundedRect(native, contact, 10.0f, contactColors[i]);
            contactRects[i] = contact;
        }

        // Right panel
        SDL_FRect rightPanel = { windowWidth * 0.35f, windowHeight * 0.05f, windowWidth * 0.6f, windowHeight * 0.9f };
        SDL_Color rightColor = darkMode ? SDL_Color{60,60,60,255} : SDL_Color{180,220,190,255};
        fillRoundedRect(native, rightPanel, 15.0f, rightColor);

        // Textbox and toggle
        textbox.draw(renderer);
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
