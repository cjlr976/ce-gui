#include "widget.h"

Widget::Widget()
    : posX(0), posY(0), width(100), height(100),
      pressed(false), onClick(nullptr),
      color({255, 255, 255, 255}),
      font(nullptr),
      hovering(false),
      toggled(false)
{}

void Widget::setPosition(int x, int y) { posX = x; posY = y; }
void Widget::setSize(int w, int h) { width = w; height = h; }
bool Widget::isInside(int x, int y) const {
    return x >= posX && x <= posX + width && y >= posY && y <= posY + height;
}
void Widget::setOnClick(WidgetCallback callback) { onClick = callback; }

void Widget::handleEvent(const SDL_Event& e) {
    if (!onClick) return;
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mx = e.button.x, my = e.button.y;
        if (isInside(mx, my)) pressed = true;
    } else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        int mx = e.button.x, my = e.button.y;
        if (pressed && isInside(mx, my)) onClick();
        pressed = false;
    }
}

void Widget::draw(Renderer& renderer) {
    SDL_Renderer* native = renderer.getNativeRenderer();
    if (!native) return;

    SDL_FRect rect = {(float)posX, (float)posY, (float)width, (float)height};

    // Fill background
    renderer.setDrawColor(color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(native, &rect);

    // Outline
    renderer.setDrawColor(255, 255, 255, 255);
    SDL_RenderRect(native, &rect);
}
