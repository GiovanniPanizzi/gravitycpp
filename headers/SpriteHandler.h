#pragma once
#include "sprites.h"
#include "Galaxy.h"
#include "TextureLoader.h"

class SpriteHandler{
    private:
    TextureLoader textureLoader;
    std::vector<Sprite> sprites;
    public:

    void createSprite(const std::string& filePath, SDL_Renderer* renderer, int width, int height);
    void updateSprites(Galaxy& currentGalaxy);
    void updateAnimatedSprites(Galaxy& currentGalaxy);
    void update(Galaxy& currentGalaxy);
};