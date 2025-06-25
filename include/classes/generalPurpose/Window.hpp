#pragma once
#include <SDL.h>
#include <stdexcept>
#include <string>

class Window {
    private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    public:
    Window(const char* title, int width, int height);
    ~Window();

    void modifySize(int width, int height);

    SDL_Window* getWindow() const;
    SDL_Renderer* getSDLRenderer() const;
};