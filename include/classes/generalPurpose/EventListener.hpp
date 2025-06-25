#pragma once
#include "Window.hpp"
#include "../../utilities/globals.hpp"

class EventListener {
private:
    SDL_Event e;
    bool arrowLeftPressed;
    bool arrowRightPressed;
    bool arrowUpPressed;
    bool leftMousePressed;
    bool escPressed;
    int mouseX, mouseY;
    Window& window;

public:
    EventListener(Window& window);
    ~EventListener(){}
    void listenEvents();
    bool isLeftPressed() const;
    bool isRightPressed() const;
    bool isUpPressed() const;
    bool isLeftMousePressed() const;
    bool isEscPressed() const;
    int getMouseX() const;
    int getMouseY() const;
};