#include "../../include/classes/EventListener.hpp"

EventListener::EventListener(Window& window) : window(window) {
    arrowLeftPressed = false;
    arrowRightPressed = false;
    arrowUpPressed = false;
    leftMousePressed = false;
    mouseX = mouseY = 0;
    escPressed = false;
}

void EventListener::listenEvents() {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            window.modifySize(e.window.data1, e.window.data2);
            screenWidth = e.window.data1;
            screenHeight = e.window.data2;
        }

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:  arrowLeftPressed = true; break;
                case SDLK_RIGHT: arrowRightPressed = true; break;
                case SDLK_UP: arrowUpPressed = true; break;
                case SDLK_ESCAPE: escPressed = true; break;
                case SDLK_a: arrowLeftPressed = true; break;
                case SDLK_d: arrowRightPressed = true; break;
                case SDLK_w: arrowUpPressed = true; break;
            }
        }

        if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:  arrowLeftPressed = false; break;
                case SDLK_RIGHT: arrowRightPressed = false; break;
                case SDLK_UP: arrowUpPressed = false; break;
                case SDLK_a: arrowLeftPressed = false; break;
                case SDLK_d: arrowRightPressed = false; break;
                case SDLK_w: arrowUpPressed = false; break;
                case SDLK_ESCAPE: escPressed = false; break;
            }
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                leftMousePressed = true;
            }
        }

        if (e.type == SDL_MOUSEBUTTONUP) {
            if (e.button.button == SDL_BUTTON_LEFT) {
                leftMousePressed = false;
            }
        }

        if (e.type == SDL_MOUSEMOTION) {
            mouseX = e.motion.x;
            mouseY = e.motion.y;
        }
    }
}

bool EventListener::isLeftPressed() const {
    return arrowLeftPressed;
}

bool EventListener::isRightPressed() const {
    return arrowRightPressed;
}

bool EventListener::isUpPressed() const {
    return arrowUpPressed;
}

bool EventListener::isLeftMousePressed() const {
    return leftMousePressed;
}

bool EventListener::isEscPressed() const {
    return escPressed;
}

int EventListener::getMouseX() const {
    return mouseX;
}

int EventListener::getMouseY() const {
    return mouseY;
}

