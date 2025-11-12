#include "toggle.h"
#include "renderer.h"
#include <SDL3/SDL.h>

ToggleSwitch::ToggleSwitch() {
    color = {180, 180, 180, 255};
    toggled = false;
}

void ToggleSwitch::draw(Renderer& renderer) {
    Widget::draw(renderer);

    SDL_Color base = toggled ? SDL_Color{0, 200, 0, 255} : SDL_Color{200, 0, 0, 255};
    if (hovering) {
        base.r = base.r + 50 > 255 ? 255 : base.r + 50;
        base.g = base.g + 50 > 255 ? 255 : base.g + 50;
        base.b = base.b + 50 > 255 ? 255 : base.b + 50;
    }

    SDL_FRect rect = {(float)posX + 2, (float)posY + 2, (float)width - 4, (float)height - 4};
    renderer.setDrawColor(base.r, base.g, base.b, base.a);
    SDL_RenderFillRect(renderer.getNativeRenderer(), &rect);
}

void ToggleSwitch::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        int mx = e.button.x, my = e.button.y;
        if (isInside(mx, my)) toggled = !toggled;
    }
    Widget::handleEvent(e);
}
