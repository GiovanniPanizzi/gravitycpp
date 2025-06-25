#include <SDL.h>
#include <iostream>

int main(){
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Main loop flag
    bool running = true;

    // Event handler
    SDL_Event e;

    // Main loop
    while (running) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }
        
        // Clear the screen (optional)
        // Here you would typically render your graphics

        // Update the screen (optional)
        // Here you would typically present your rendered graphics
    }

    // Destroy window
    SDL_DestroyWindow(window);
    
    // Quit SDL subsystems
    SDL_Quit();

    return 0;   
}