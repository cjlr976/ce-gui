#ifndef LAYOUT_H
#define LAYOUT_H

#include "renderer.h"
#include "widget.h"
#include "button.h"
#include "label.h"
#include "slider.h"
#include "toggle.h"
#include "textbox.h"

class Layout : public Widget {
public:
    Layout();
    void setWidget(Widget* widget);
    void draw(Renderer& renderer) override;

private:
    Widget* currentWidget;
};

#endif
