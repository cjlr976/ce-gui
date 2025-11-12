#ifndef LABEL_H
#define LABEL_H

#include <SDL3/SDL.h>        // SDL core
#include <SDL3_ttf/SDL_ttf.h> // SDL_ttf functions (TTF_GetError, TTF_RenderText_Blended)
#include <iostream>           // std::cerr, std::endl
#include "renderer.h"
#include "widget.h"
#include "button.h"
#include "label.h"
#include "slider.h"
#include "toggle.h"
#include "textbox.h"

class Label : public Widget {
public:
    Label(Renderer* renderer, TTF_Font* font, const std::string& text);
    void setText(const std::string& text);
    void setColor(SDL_Color c);
    virtual void draw(Renderer& renderer) override;

private:
    Renderer* rendererPtr;
    TTF_Font* font;
    std::string label;
    SDL_Color textColor;
};

#endif
