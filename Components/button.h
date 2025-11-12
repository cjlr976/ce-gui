#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"
#include <string>

class Button : public Widget {
public:
    Button(Renderer* renderer, TTF_Font* font, const std::string& text);
    void draw(Renderer& renderer) override;
    void setText(const std::string& text);
    

private:
    std::string label;
};


#endif
