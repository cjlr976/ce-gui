// Note: Download SDL3_image and SDL3_ttf first before running this code
// Run this command g++ textbox.cpp -o textbox.exe -I "C:\SDL\x86_64-w64-mingw32\include" -L "C:\SDL\x86_64-w64-mingw32\lib" -lSDL3_ttf -lSDL3
// and ./textbox.exe in terminal

#define SDL_MAIN_USE_CALLBACKS //   String → Surface → Texture → Renderer → Screen
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <filesystem>
#include <string>
using namespace std;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
TTF_Font* font = nullptr;

int windowWidth = 800;
int windowHeight = 600;
string userInput = "";

bool cursorVisible = true;
Uint32 lastCursorToggle = 0;
Uint32 cursorBlinkInterval = 500; // milliseconds
float textScrollOffset = 0.0f;

// Store positions of small rectangles for click detection
SDL_FRect smallRects[3];

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_WINDOW_RESIZED) {
        windowWidth = event->window.data1;
        windowHeight = event->window.data2;
        cout << "Window resized to " << windowWidth << " x " << windowHeight << endl;
    }

    // Handle user input
    if (event->type == SDL_EVENT_TEXT_INPUT) {
        userInput += event->text.text;
    }

    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_BACKSPACE && !userInput.empty()) {
        userInput.pop_back();
    }

    // Handle mouse clicks on small rectangles
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mouseX = event->button.x;
        int mouseY = event->button.y;

        for (int i = 0; i < 3; i++) {
            SDL_FRect &rect = smallRects[i];
            if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                mouseY >= rect.y && mouseY <= rect.y + rect.h) {
                // Clicked on small rectangle, reset text
                userInput = "";
                textScrollOffset = 0.0f;
                break;
            }
        }
    }

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Big rectangle
    float big_squareWidth = windowWidth * 0.85f;
    float big_squareHeight = windowHeight * 0.95f;
    SDL_FRect bigSquare = {
        windowWidth * 0.07f,
        (windowHeight - big_squareHeight) / 2.0f,
        big_squareWidth,
        big_squareHeight
    };
    SDL_RenderRect(renderer, &bigSquare);

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
        SDL_RenderRect(renderer, &smallRect);
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
    SDL_RenderRect(renderer, &avgRect);

    // Text input rectangle
    float text_squareWidth = windowWidth * 0.3f;
    float text_squareHeight = windowHeight * 0.05f;
    SDL_FRect textRect = {
        bigSquare.x + big_squareWidth * 0.53f,
        bigSquare.y + (big_squareHeight - text_squareHeight) / 1.1f,
        text_squareWidth,
        text_squareHeight
    };
    SDL_RenderRect(renderer, &textRect);

    // Render user text with scrolling and blinking cursor
    if (font) {
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* textSurface = nullptr;
        if (!userInput.empty()) {
            textSurface = TTF_RenderText_Solid(font, userInput.c_str(), userInput.size(), white);
        } else {
            textSurface = TTF_RenderText_Solid(font, " ", 1, white);
        }

        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            // Scroll text if it exceeds box width
            float maxWidth = textRect.w - 10; // padding inside box
            float textWidth = (float)textSurface->w;

            if (textWidth > maxWidth) {
                textScrollOffset = textWidth - maxWidth;
            } 
            else {
                textScrollOffset = 0;
            }

            // Apply offset to text position
            SDL_FRect textBox = {
                textRect.x + 5 - textScrollOffset,
                textRect.y + (text_squareHeight - textSurface->h) / 2.0f,
                textWidth,
                (float)textSurface->h
            };

            SDL_RenderTexture(renderer, textTexture, NULL, &textBox);

            // Draw blinking cursor
            if (cursorVisible) {
                SDL_FRect cursorRect = {
                    textBox.x + textBox.w + 2,
                    textBox.y,
                    2,
                    textBox.h
                };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &cursorRect);
            }

            SDL_DestroyTexture(textTexture);
            SDL_DestroySurface(textSurface); // SDL3 fix
        }
    }

    SDL_RenderPresent(renderer);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    Uint32 now = SDL_GetTicks();
    if (now - lastCursorToggle > cursorBlinkInterval) {
        cursorVisible = !cursorVisible;
        lastCursorToggle = now;
    }

    render();
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        cerr << "Error initializing SDL: " << SDL_GetError() << endl;
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        cerr << "Error initializing TTF: " << SDL_GetError() << endl;
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("GUI", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        cerr << "Error creating window: " << SDL_GetError() << endl;
        return SDL_APP_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        cerr << "Error creating renderer: " << SDL_GetError() << endl;
        return SDL_APP_FAILURE;
    }

    font = TTF_OpenFont("Roboto_SemiCondensed-Black.ttf", 24);
    if (!font) {
        cerr << "Error loading font: " << SDL_GetError() << endl;
        return SDL_APP_FAILURE;
    }

    SDL_StartTextInput(window);
    return SDL_APP_CONTINUE;
}