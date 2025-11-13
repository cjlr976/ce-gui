#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "widget.h"

class TextBox : public Widget {
public:
    TextBox(Renderer* renderer, TTF_Font* font);
    ~TextBox() override = default;

    void draw(Renderer& renderer) override;
    void handleEvent(const SDL_Event& e) override;

    void setBox(int x, int y, int w, int h);
    void clear();

private:
    Renderer* rendererPtr;
    TTF_Font* font;
    std::string text;

    bool cursorVisible;
    Uint32 lastCursorToggle;
    Uint32 cursorBlinkInterval; // ms

    float textScrollOffset;

    // create a texture for current text; returns nullptr on failure; outW/outH populated if non-null
    SDL_Texture* createTextTexture(int* outW = nullptr, int* outH = nullptr);
};

#endif