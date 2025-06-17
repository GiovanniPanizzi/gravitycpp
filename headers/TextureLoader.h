#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "sprites.h"

class TextureLoader {
public:
    SDL_Texture* loadTexture(const std::string& filePath, SDL_Renderer* renderer);
};