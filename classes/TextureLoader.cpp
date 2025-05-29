#include "../headers/TextureLoader.h"
#include <iostream>

SDL_Texture* TextureLoader::loadTexture(const std::string& filePath, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "IMG_Load error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return texture;
}

