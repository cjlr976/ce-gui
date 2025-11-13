#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"
#include <string>
#include <functional>

class Button : public Widget {
public:
    Button(Renderer* renderer, TTF_Font* f, const std::string& text);

    void setText(const std::string& text);
    void draw(Renderer& renderer);

    // Replace plain function pointer with std::function
    std::function<void()> onClick;

    void handleEvent(const SDL_Event& e) {
        if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            int mx = e.button.x, my = e.button.y;
            if (isInside(mx, my) && onClick) onClick();  // call the lambda
        }
    }

private:
    TTF_Font* font;
    std::string label;
    SDL_Color color;
};



#endif
