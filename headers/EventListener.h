#pragma once
#include <SDL.h>
#include "globals.h"

class EventListener {
private:
    SDL_Event e;
    bool arrowLeftPressed;
    bool arrowRightPressed;
    bool arrowUpPressed;
    bool leftMousePressed;
    int mouseX, mouseY;

public:
    EventListener();
    ~EventListener(){}
    void listenEvents();
    bool isLeftPressed() const;
    bool isRightPressed() const;
    bool isUpPressed() const;
    bool isLeftMousePressed() const;
    int getMouseX() const;
    int getMouseY() const;
};