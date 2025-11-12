#ifndef SLIDER_H
#define SLIDER_H

#include "widget.h"

class Slider : public Widget {
public:
    Slider();
    void draw(Renderer& renderer) override;
    void handleEvent(const SDL_Event& e) override;
    void setRange(float minVal, float maxVal);

private:
    float minVal, maxVal, value;
    bool dragging;
};

#endif
