#include "Widget.h"
#include "Renderer.h"

Widget::Widget() {
    posX = 0;
    posY = 0;
    width = 100;
    height = 100;
}

void Widget::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void Widget::setSize(int w, int h) {
    width = w;
    height = h;
}

void Widget::draw(Renderer& renderer) {
    // Default widget is a red rectangle
    renderer.setDrawColor(255, 0, 0, 255);
    renderer.fillRect(posX, posY, width, height);
}
