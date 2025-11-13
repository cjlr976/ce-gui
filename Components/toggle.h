#pragma once
#include "widget.h"
#include "renderer.h"
#include <SDL3/SDL.h>
#include <functional>

class ToggleSwitch : public Widget {
public:
    ToggleSwitch();

    void draw(Renderer& renderer) override;
    void handleEvent(const SDL_Event& e) override;

    bool toggled;                          // current toggle state
    std::function<void(bool)> onToggle;    // callback called when toggled
};
