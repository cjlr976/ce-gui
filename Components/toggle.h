#ifndef TOGGLE_H
#define TOGGLE_H

#include "widget.h"

class ToggleSwitch : public Widget {
public:
    ToggleSwitch();
    void draw(Renderer& renderer) override;
    void handleEvent(const SDL_Event& e) override;
};

#endif
