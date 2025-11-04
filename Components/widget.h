#ifndef WIDGET_H
#define WIDGET_H

#include <SDL3/SDL.h>

class Renderer;

class Widget {
public:
    Widget();
    virtual ~Widget() {}

    virtual void draw(Renderer& renderer);

    void setPosition(int x, int y);
    void setSize(int w, int h);

protected:
    int posX;
    int posY;
    int width;
    int height;
};

#endif
