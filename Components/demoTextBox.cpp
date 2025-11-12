#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "renderer.h"
#include "layout.h"
#include "widget.h"
#include "textbox.h"

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

    SDL_Window* window = SDL_CreateWindow("SDL3 Widget TextBox", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Renderer renderer(window);


    // Load a font (make sure file exists in working directory)
    TTF_Font* font = TTF_OpenFont("Roboto_SemiCondensed-Black.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << SDL_GetError() << std::endl;
        // Not fatal for demo, but textbox rendering will be skipped
    }

    // Create a TextBox widget
    TextBox textbox(&renderer, font);

    // Position and size similar to the prior standalone textbox demo
    int bigW = (int)(windowWidth * 0.85f);
    int bigH = (int)(windowHeight * 0.95f);
    int bigX = (int)(windowWidth * 0.07f);
    int bigY = (windowHeight - bigH) / 2;

    // place the textbox inside the avg rect area as in prior demo
    int tbW = (int)(windowWidth * 0.3f);
    int tbH = (int)(windowHeight * 0.05f);
    int tbX = bigX + (int)(bigW * 0.53f);
    int tbY = bigY + (bigH - tbH) / 2;

    textbox.setBox(tbX, tbY, tbW, tbH);

    // Start text input
    SDL_StartTextInput(window);

    bool running = true;
    SDL_Event e;

    // store small rectangles from layout so clicks can clear the textbox (same behavior as demo)
    SDL_FRect smallRects[3];

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                windowWidth = e.window.data1;
                windowHeight = e.window.data2;

                // recompute layout and textbox geometry
                bigW = (int)(windowWidth * 0.85f);
                bigH = (int)(windowHeight * 0.95f);
                bigX = (int)(windowWidth * 0.07f);
                bigY = (windowHeight - bigH) / 2;

                tbW = (int)(windowWidth * 0.3f);
                tbH = (int)(windowHeight * 0.05f);
                tbX = bigX + (int)(bigW * 0.53f);
                tbY = bigY + (bigH - tbH) / 2;
                textbox.setBox(tbX, tbY, tbW, tbH);
            }

            // allow textbox to process text events / keyboard
            textbox.handleEvent(e);

            // Additional click handling to clear text when small rects are clicked
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int mx = e.button.x;
                int my = e.button.y;
                for (int i = 0; i < 3; ++i) {
                    SDL_FRect &r = smallRects[i];
                    if (mx >= r.x && mx <= r.x + r.w && my >= r.y && my <= r.y + r.h) {
                        textbox.clear();
                        break;
                    }
                }
            }
        }

        // render layout (similar to original textbox demo)
        renderer.setDrawColor(0, 0, 0, 255);
        renderer.clear();

        SDL_Renderer* native = renderer.getNativeRenderer();

        // Big rectangle outline
        float big_squareWidth = windowWidth * 0.85f;
        float big_squareHeight = windowHeight * 0.95f;
        SDL_FRect bigSquare = {
            windowWidth * 0.07f,
            (windowHeight - big_squareHeight) / 2.0f,
            big_squareWidth,
            big_squareHeight
        };
        renderer.setDrawColor(255, 255, 255, 255);
        SDL_RenderRect(native, &bigSquare);

        // Small rectangles
        float small_squareWidth = windowWidth * 0.25f;
        float small_squareHeight = windowHeight * 0.1f;
        int nSmall = 3;
        float spacing = (big_squareHeight - nSmall * small_squareHeight) / (nSmall + 1);

        for (int i = 0; i < nSmall; i++) {
            SDL_FRect smallRect = {
                bigSquare.x + big_squareWidth * 0.1f,
                bigSquare.y + spacing * (i + 1) + small_squareHeight * i,
                small_squareWidth,
                small_squareHeight
            };
            SDL_RenderRect(native, &smallRect);
            smallRects[i] = smallRect; // store for click detection
        }

        // Average rectangle
        float avg_squareWidth = windowWidth * 0.35f;
        float avg_squareHeight = windowHeight * 0.85f;
        SDL_FRect avgRect = {
            bigSquare.x + big_squareWidth * 0.5f,
            bigSquare.y + (big_squareHeight - avg_squareHeight) / 2.0f,
            avg_squareWidth,
            avg_squareHeight
        };
        SDL_RenderRect(native, &avgRect);

        // Draw the textbox widget (it handles its own outline and text)
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