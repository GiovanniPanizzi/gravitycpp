#include "../headers/Menu.h"
#include <iostream>

Menu::Menu(std::vector<Vec2> positions, std::vector<Size> sizes, std::vector<Color> colors) {
    for(size_t i = 0; i < positions.size(); i++) {
        buttonsPositions.push_back(positions[i]);
        buttonsSizes.push_back(sizes[i]);
        buttonsColors.push_back(colors[i]);
        buttons.push_back({i});
    }
}

int Menu::handleClicks(EventListener& eventListener) {
    if(eventListener.isLeftMousePressed()) {
        for(size_t i = 0; i < buttons.size(); i++) {
            Vec2 pos = buttonsPositions[i];
            Size size = buttonsSizes[i];
            if(eventListener.getMouseX() >= pos.x && eventListener.getMouseX() <= pos.x + size.width &&
               eventListener.getMouseY() >= pos.y && eventListener.getMouseY() <= pos.y + size.height) {
                return i + 1;
            }
        }
    }
    return 0;
}

void Menu::draw(Draw& draw) {
    for(size_t i = 0; i < buttons.size(); i++) {
        Vec2 pos = buttonsPositions[i];
        Size size = buttonsSizes[i];
        Color color = buttonsColors[i];
        draw.drawRect(pos.x, pos.y, size.width, size.height, color.r, color.g, color.b, color.a);
    }
}

