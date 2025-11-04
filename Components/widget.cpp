#include "Widget.h"
#include <iostream>

Widget::Widget() : posX(0), posY(0), width(100), height(100), pressed(false), onClick(nullptr) {}

void Widget::setPosition(int x, int y) { posX = x; posY = y; }
void Widget::setSize(int w, int h) { width = w; height = h; }

bool Widget::isInside(int x, int y) const {
    return x >= posX && x <= posX + width &&
           y >= posY && y <= posY + height;
}

void Widget::draw(Renderer& renderer) {
    if (pressed)
        renderer.setDrawColor(200, 0, 0, 255); // pressed color
    else
        renderer.setDrawColor(255, 0, 0, 255); // default color

    renderer.fillRect(posX, posY, width, height);
}

void Widget::handleEvent(const SDL_Event& e) {
    if (!onClick) return; // Only handle click if a callback is set

    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mx = e.button.x;
        int my = e.button.y;
        if (isInside(mx, my)) pressed = true;
    } else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        int mx = e.button.x;
        int my = e.button.y;
        if (pressed && isInside(mx, my)) onClick();
        pressed = false;
    }
}

void Widget::setOnClick(WidgetCallback callback) {
    onClick = callback;
}
