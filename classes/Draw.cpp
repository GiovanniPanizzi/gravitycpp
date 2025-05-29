#include "../headers/Draw.h"
#include <iostream>
#include <cmath>

Draw::Draw(SDL_Renderer& renderer) {
    this -> renderer = &renderer;
}

Draw::~Draw(){}

void Draw::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

void Draw::drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

void Draw::drawCircle(int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int w = 0; w < 360; w++) {
        float angle = w * M_PI / 180.0f;
        int x = static_cast<int>(centerX + radius * cos(angle));
        int y = static_cast<int>(centerY + radius * sin(angle));
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void Draw::drawFilledRotatedRect(int x, int y, int w, int h, double angleDeg, int pivotX, int pivotY, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Texture* tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, tex);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);

    SDL_Rect dstRect = { x, y, w, h };
    SDL_Point center = { pivotX, pivotY };
    SDL_RenderCopyEx(renderer, tex, nullptr, &dstRect, angleDeg, &center, SDL_FLIP_NONE);

    SDL_DestroyTexture(tex);
}

void Draw::drawFilledCircle(int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    for (int y = -radius; y <= radius; y++) {
        int dx = static_cast<int>(sqrt(radius * radius - y * y));
        int x1 = centerX - dx;
        int x2 = centerX + dx;
        SDL_RenderDrawLine(renderer, x1, centerY + y, x2, centerY + y);
    }
}

/*void Draw::drawPlatform(float centerX, float centerY, float innerR, float outerR, float startAngle, float endAngle, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    const float degToRad = M_PI / 180.0f;

    float angleStep = 0.1f;
    float radiusStep = 1.0f;

    for (float angle = startAngle; angle <= endAngle; angle += angleStep) {
        float theta = angle * degToRad;

        for (float radius = innerR; radius <= outerR; radius += radiusStep) {
            int x = static_cast<int>(centerX + radius * cos(theta));
            int y = static_cast<int>(centerY + radius * sin(theta)); 
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}*/

void Draw::drawPlatform(float centerX, float centerY, float innerR, float outerR, float startAngle, float endAngle, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    const float degToRad = M_PI / 180.0f;

    // Step size for angles (adjust for performance vs. quality)
    float angleStep = 0.5 / (outerR - innerR); // Adjust this value for smoother curves

    // Vectors to store points along the inner and outer radii
    std::vector<SDL_Point> innerPoints;
    std::vector<SDL_Point> outerPoints;

    // Calculate points along the inner and outer radii
    for (float angle = startAngle; angle <= endAngle; angle += angleStep) {
        float theta = angle * degToRad;

        // Inner radius point
        int innerX = static_cast<int>(centerX + innerR * cos(theta));
        int innerY = static_cast<int>(centerY + innerR * sin(theta));
        innerPoints.push_back({innerX, innerY});

        // Outer radius point
        int outerX = static_cast<int>(centerX + outerR * cos(theta));
        int outerY = static_cast<int>(centerY + outerR * sin(theta));
        outerPoints.push_back({outerX, outerY});
    }

    // Draw lines connecting consecutive points along the inner and outer radii
    for (size_t i = 1; i < innerPoints.size(); i++) {
        SDL_RenderDrawLine(renderer, innerPoints[i - 1].x, innerPoints[i - 1].y, innerPoints[i].x, innerPoints[i].y);
        SDL_RenderDrawLine(renderer, outerPoints[i - 1].x, outerPoints[i - 1].y, outerPoints[i].x, outerPoints[i].y);
    }

    // Optionally, fill the area between the inner and outer radii
    for (size_t i = 0; i < innerPoints.size() - 1; i++) {
        SDL_RenderDrawLine(renderer, innerPoints[i].x, innerPoints[i].y, outerPoints[i].x, outerPoints[i].y);
    }
}


void Draw::drawRotatedSprite(const Sprite& sprite) {
    SDL_RendererFlip flip = sprite.facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

    SDL_RenderCopyEx(renderer,
                     sprite.texture,
                     &sprite.srcRect,
                     &sprite.dstRect,
                     sprite.angle,
                     &sprite.pivot,
                     flip);
}

void Draw::present() {
    SDL_RenderPresent(renderer);
}