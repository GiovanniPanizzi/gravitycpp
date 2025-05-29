#pragma once
#include <SDL_image.h>
#include <SDL.h>

struct Sprite {
    SDL_Texture* texture;
    SDL_Rect srcRect;               
    SDL_Rect dstRect;
    SDL_Point pivot;
    float angle;
    bool facingRight;        
};

struct AnimatedSprite {
    Sprite* sprite;
    int currentFrame;
    bool created;
};