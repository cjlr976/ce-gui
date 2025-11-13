#include "slider.h"
#include "renderer.h"
#include <SDL3/SDL.h>

Slider::Slider() : minVal(0), maxVal(100), value(50), dragging(false) {
    color = {100, 100, 255, 255};
}

void Slider::setRange(float minV, float maxV) { minVal = minV; maxVal = maxV; }

void Slider::draw(Renderer& renderer) {
    Widget::draw(renderer);
    SDL_FRect bar = {(float)posX, (float)(posY + height / 2 - 5), (float)width, 10};
    renderer.setDrawColor(150, 150, 150, 255);
    SDL_RenderFillRect(renderer.getNativeRenderer(), &bar);

    float knobX = posX + (value - minVal) / (maxVal - minVal) * width;
    SDL_FRect knob = {knobX - 5, (float)posY, 10, (float)height};
    renderer.setDrawColor(255, 255, 255, 255);
    SDL_RenderFillRect(renderer.getNativeRenderer(), &knob);
}

void Slider::handleEvent(const SDL_Event& e) {
    int mx = e.type == SDL_EVENT_MOUSE_BUTTON_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_UP ? e.button.x : 0;
    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && isInside(mx, e.button.y)) dragging = true;
    else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) dragging = false;

    if (dragging && e.type == SDL_EVENT_MOUSE_MOTION) {
        value = minVal + ((float)e.motion.x - posX) / width * (maxVal - minVal);
        if (value < minVal) value = minVal;
        if (value > maxVal) value = maxVal;
    }
    Widget::handleEvent(e);
}
