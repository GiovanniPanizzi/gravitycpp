#include "../headers/SpriteHandler.h"
#include <iostream>

void SpriteHandler::createSprite(const std::string& filePath, SDL_Renderer* renderer, int width, int height) {
    SDL_Texture* texture = textureLoader.loadTexture(filePath.c_str(), renderer);
    if (!texture) return;

    Sprite sprite {
        texture,
        SDL_Rect{0, 0, width, height},
        SDL_Rect{0, 0, 0, 0},
        SDL_Point{0, 0},
        0.0f,                         
        true                        
    };  
    sprites.push_back(sprite);
}

void SpriteHandler::updateSprites(Galaxy& currentGalaxy){
    for(size_t i = 0; i < currentGalaxy.entities.size(); i++){
        size_t entityIndex = currentGalaxy.entities[i].index;
        AnimatedSprite& animatedSprite = currentGalaxy.animatedSpriteMap[entityIndex];
        if(currentGalaxy.animatedSpriteMap[entityIndex].created){
            animatedSprite.sprite -> angle = currentGalaxy.angles[entityIndex].deg;
            int drawX = currentGalaxy.positions[entityIndex].x - currentGalaxy.sizes[entityIndex].width / 2 - currentGalaxy.positions[0].x + screenWidth / 2; 
            int drawY = currentGalaxy.positions[entityIndex].y - currentGalaxy.sizes[entityIndex].height - currentGalaxy.positions[0].y + screenHeight / 2;
            animatedSprite.sprite->dstRect.x = drawX;
            animatedSprite.sprite->dstRect.y = drawY;
            int x = animatedSprite.currentFrame % 3;
            int y = animatedSprite.currentFrame / 3;
            animatedSprite.sprite->srcRect.x = x * animatedSprite.sprite->srcRect.w;
            animatedSprite.sprite->srcRect.y = y * animatedSprite.sprite->srcRect.h;
        }
        else{
            //animatedSprite.created = true;
            //animatedSprite.sprite = &sprites[0];
            //animatedSprite.sprite -> pivot = {currentGalaxy.sizes[entityIndex].width / 2, currentGalaxy.sizes[entityIndex].height};
            //animatedSprite.sprite -> angle = currentGalaxy.angles[entityIndex].deg;
            //animatedSprite.sprite -> dstRect.w = currentGalaxy.sizes[entityIndex].width;
            //animatedSprite.sprite -> dstRect.h = currentGalaxy.sizes[entityIndex].height;
        }
    }
}

void SpriteHandler::updateAnimatedSprites(Galaxy& currentGalaxy){
}

void SpriteHandler::update(Galaxy& currentGalaxy){
    updateSprites(currentGalaxy);
}