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
    bool escPressed;
    int mouseX, mouseY;
    SDL_Renderer* renderer;

public:
    EventListener(SDL_Renderer* renderer);
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