#ifndef LAYOUT_H
#define LAYOUT_H

#include "Widget.h"

class Layout : public Widget {
public:
    Layout();
    void setWidget(Widget* widget);
    void draw(Renderer& renderer) override;

private:
    Widget* currentWidget;
};

#endif
