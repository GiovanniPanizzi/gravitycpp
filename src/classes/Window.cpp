#include "../../include/classes/Window.hpp"

Window::Window(const char* title, int width, int height) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
    }

    // Create a window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }

    // Create a renderer for the window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        throw std::runtime_error("Renderer could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
}

Window::~Window() {
    // Destroy the renderer
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    
    // Destroy the window
    if (window) {
        SDL_DestroyWindow(window);
    }
    
    // Quit SDL subsystems
    SDL_Quit();
}

void Window::modifySize(int width, int height) {
    if (window) {
        SDL_SetWindowSize(window, width, height);
    }
}

SDL_Window* Window::getWindow() const {
    return window;
}

SDL_Renderer* Window::getSDLRenderer() const {
    return renderer;
}

