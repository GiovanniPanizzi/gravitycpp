#include "../../../include/classes/generalPurpose/Draw.hpp"

Draw::Draw(Window& window) : window(window) {}
Draw::~Draw() {}
void Draw::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(window.getSDLRenderer(), r, g, b, a);
    SDL_RenderClear(window.getSDLRenderer());
}
void Draw::drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(window.getSDLRenderer(), r, g, b, a);
    SDL_RenderDrawRect(window.getSDLRenderer(), &rect);
}
void Draw::drawCircle(int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(window.getSDLRenderer(), r, g, b, a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(window.getSDLRenderer(), centerX + dx, centerY + dy);
            }
        }
    }
}
void Draw::drawFilledRotatedRect(int x, int y, int w, int h, double angleRad, int pivotX, int pivotY, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(window.getSDLRenderer(), r, g, b, a);
    SDL_Rect rect = {x - pivotX, y - pivotY, w, h};
    SDL_RenderFillRect(window.getSDLRenderer(), &rect);
}
void Draw::drawFilledCircle(int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(window.getSDLRenderer(), r, g, b, a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(window.getSDLRenderer(), centerX + dx, centerY + dy);
            }
        }
    }
}
void Draw::drawAnnularSection(int centerX, int centerY, int innerRadius, int outerRadius,
                               float startAngleRad, float endAngleRad,
                               Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Renderer* renderer = window.getSDLRenderer();

    const float angleStep = 1.0f;

    for (float angle = startAngleRad; angle < endAngleRad; angle += angleStep) {
        float nextAngle = angle + angleStep;
        if (nextAngle > endAngleRad) nextAngle = endAngleRad;

        // Calcola i 4 punti dell'anello tra due angoli
        float cosA1 = cosf(angle);
        float sinA1 = sinf(angle);
        float cosA2 = cosf(nextAngle);
        float sinA2 = sinf(nextAngle);

        SDL_Vertex vertices[6];

        // Punto interno 1
        vertices[0].position = { centerX + innerRadius * cosA1, centerY + innerRadius * sinA1 };
        vertices[0].color = { r, g, b, a };

        // Punto esterno 1
        vertices[1].position = { centerX + outerRadius * cosA1, centerY + outerRadius * sinA1 };
        vertices[1].color = { r, g, b, a };

        // Punto esterno 2
        vertices[2].position = { centerX + outerRadius * cosA2, centerY + outerRadius * sinA2 };
        vertices[2].color = { r, g, b, a };

        // Punto interno 1 (di nuovo per secondo triangolo)
        vertices[3] = vertices[0];

        // Punto esterno 2 (di nuovo)
        vertices[4] = vertices[2];

        // Punto interno 2
        vertices[5].position = { centerX + innerRadius * cosA2, centerY + innerRadius * sinA2 };
        vertices[5].color = { r, g, b, a };

        SDL_RenderGeometry(renderer, nullptr, vertices, 6, nullptr, 0);
    }
}
void Draw::present() {
    SDL_RenderPresent(window.getSDLRenderer());
}