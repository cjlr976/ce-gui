#include "Layout.h"

Layout::Layout() {
    currentWidget = nullptr;
}

void Layout::setWidget(Widget* widget) {
    currentWidget = widget;
}

void Layout::draw(Renderer& renderer) {
    if (currentWidget) {
        currentWidget->draw(renderer);
    }
}
