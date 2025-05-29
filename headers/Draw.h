#pragma once
#include <SDL.h>
#include "sprites.h"
class Draw {
    private:
    SDL_Renderer* renderer;

    public:
    Draw(SDL_Renderer& renderer);
    virtual ~Draw();

    void clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawCircle(int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawFilledRotatedRect(int x, int y, int w, int h, double angleDeg, int pivotX, int pivotY, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawFilledCircle(int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawPlatform(float centerX, float centerY, float innerRadius, float outerRadius,float startAngleDeg, float endAngleDeg, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void drawRotatedSprite(const Sprite& sprite);
    void present();
};