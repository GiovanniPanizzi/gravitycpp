#include "../../../include/classes/generalPurpose/Window.hpp"

Window::Window(const char* title, int width, int height) {
    // Initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
    }

    // Set OpenGL version and profile (3.3 Core)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create a window with OpenGL flag
    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }

    // Create OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        throw std::runtime_error("OpenGL context could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }

    // Initialize glad by loading OpenGL function pointers
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // Optionally enable VSync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        // Warning only, VSync not supported
    }

    // No need for SDL_Renderer when using OpenGL directly
    renderer = nullptr;
}

Window::~Window() {
    // Destroy OpenGL context
    SDL_GL_DeleteContext(SDL_GL_GetCurrentContext());

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
        glViewport(0, 0, width, height);
    }
}

SDL_Window* Window::getWindow() const {
    return window;
}

SDL_Renderer* Window::getSDLRenderer() const {
    return renderer; // will be nullptr
}

