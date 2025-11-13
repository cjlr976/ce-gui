#ifndef WIDGET_H
#define WIDGET_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include "renderer.h"

typedef void (*WidgetCallback)();

class Renderer;

class Widget {
public:
    Widget();
    virtual ~Widget() {}

    virtual void draw(Renderer& renderer);
    virtual void handleEvent(const SDL_Event& e);

    void setPosition(int x, int y);
    void setSize(int w, int h);
    bool isInside(int x, int y) const;

    void setOnClick(WidgetCallback callback);

    // Shared properties for all widgets
    SDL_Color color;
    TTF_Font* font;
    bool hovering;
    bool toggled;

protected:
    int posX, posY;
    int width, height;
    bool pressed;
    WidgetCallback onClick;
};

#endif
