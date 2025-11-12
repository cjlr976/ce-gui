#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "renderer.h"
#include "widget.h"
#include "button.h"
#include "label.h"
#include "slider.h"
#include "toggle.h"
#include "textbox.h"

// Global pointer for input box so the button callback can access it
TextBox* g_inputBox = nullptr;

// Button callback
void sendMessage() {
    std::cout << "Sending message: " << std::endl;
    if (g_inputBox) g_inputBox->clear();
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() != 0) {
        std::cerr << "TTF Init failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    int windowWidth = 800;
    int windowHeight = 600;
    SDL_Window* window = SDL_CreateWindow("WhatsApp Clone Demo", windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Renderer renderer(window);

    // Load font
    TTF_Font* font = TTF_OpenFont("Roboto_SemiCondensed-Black.ttf", 20);
    if (!font) {
        std::cerr << "Failed to load font" << std::endl;
    }

    // WhatsApp UI panels
    SDL_FRect chatsPanel = {0, 0, windowWidth * 0.3f, (float)windowHeight};
    SDL_FRect chatPanel = {chatsPanel.w, 0, windowWidth - chatsPanel.w, (float)windowHeight};

    // Left panel chat labels
    Label chat1(&renderer, font, "Alice");
    chat1.setPosition(10, 50);
    chat1.setSize(chatsPanel.w - 20, 40);

    Label chat2(&renderer, font, "Bob");
    chat2.setPosition(10, 100);
    chat2.setSize(chatsPanel.w - 20, 40);

    // Textbox for message input
    TextBox inputBox(&renderer, font);
    g_inputBox = &inputBox; // set global pointer for callback
    inputBox.setPosition(chatPanel.x + 10, windowHeight - 60);
    inputBox.setSize(chatPanel.w - 80, 50);

    // Send button
    Button sendButton(&renderer, font, "Send");
    sendButton.setPosition(chatPanel.x + chatPanel.w - 60, windowHeight - 60);
    sendButton.setSize(50, 50);
    sendButton.setOnClick(sendMessage);

    // Sample chat messages
    Label message1(&renderer, font, "Alice: Hi there!");
    message1.setPosition(chatPanel.x + 10, 20);
    message1.setSize(chatPanel.w - 20, 30);

    bool running = true;
    SDL_Event e;
    SDL_StartTextInput(window);

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            if (e.type == SDL_EVENT_WINDOW_RESIZED) {
                windowWidth = e.window.data1;
                windowHeight = e.window.data2;

                chatsPanel.w = windowWidth * 0.3f;
                chatPanel.x = chatsPanel.w;
                chatPanel.w = windowWidth - chatsPanel.w;

                inputBox.setPosition(chatPanel.x + 10, windowHeight - 60);
                inputBox.setSize(chatPanel.w - 80, 50);

                sendButton.setPosition(chatPanel.x + chatPanel.w - 60, windowHeight - 60);
                sendButton.setSize(50, 50);
            }

            // Handle widget events
            inputBox.handleEvent(e);
            sendButton.handleEvent(e);
        }

        // Render background
        renderer.setDrawColor(240, 240, 240, 255);
        renderer.clear();

        SDL_Renderer* native = renderer.getNativeRenderer();

        // Left panel background
        renderer.setDrawColor(220, 220, 220, 255);
        SDL_FRect leftRect = {chatsPanel.x, chatsPanel.y, chatsPanel.w, chatsPanel.h};
        SDL_RenderFillRect(native, &leftRect);

        // Right panel background
        renderer.setDrawColor(255, 255, 255, 255);
        SDL_FRect rightRect = {chatPanel.x, chatPanel.y, chatPanel.w, chatPanel.h};
        SDL_RenderFillRect(native, &rightRect);

        // Draw chat labels
        chat1.draw(renderer);
        chat2.draw(renderer);

        // Draw chat messages
        message1.draw(renderer);

        // Draw input and send button
        inputBox.draw(renderer);
        sendButton.draw(renderer);

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
