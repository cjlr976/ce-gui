#ifndef WIDGET_H
#define WIDGET_H

#include <SDL3/SDL.h>
#include "Renderer.h"

typedef void (*WidgetCallback)();

class Widget {
public:
    Widget();
    virtual ~Widget() {}

    virtual void draw(Renderer& renderer);

    // Event handling including optional click detection
    virtual void handleEvent(const SDL_Event& e);

    void setPosition(int x, int y);
    void setSize(int w, int h);
    bool isInside(int x, int y) const;

    // Button-like behavior
    void setOnClick(WidgetCallback callback);

protected:
    int posX, posY;
    int width, height;
    bool pressed;
    WidgetCallback onClick;
};

#endif
