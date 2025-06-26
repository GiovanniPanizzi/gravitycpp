#include "../../../include/classes/generalPurpose/Draw.hpp"
#include <vector>

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
void Draw::drawFilledRotatedRect(int x, int y, int w, int h, double angleDeg, int pivotX, int pivotY, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Renderer* renderer = window.getSDLRenderer();
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

    const int segments = 60; // Numero di segmenti per l'intero cerchio
    float totalAngle = endAngleRad - startAngleRad;
    int numArcSegments = static_cast<int>(std::ceil(totalAngle / (2 * M_PI) * segments));
    if (numArcSegments < 2) numArcSegments = 2; // Minimo 2 segmenti per un inizio e fine

    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    // Aggiungi i vertici
    for (int i = 0; i <= numArcSegments; ++i) {
        float currentAngle = startAngleRad + (static_cast<float>(i) / numArcSegments) * totalAngle;

        // Vertice sul raggio interno
        SDL_Vertex innerVert;
        innerVert.position.x = static_cast<float>(centerX + innerRadius * std::cos(currentAngle));
        innerVert.position.y = static_cast<float>(centerY + innerRadius * std::sin(currentAngle));
        innerVert.color = {r, g, b, a};
        innerVert.tex_coord = {0.0f, 0.0f}; // Non usiamo texture, ma è buona norma inizializzare
        vertices.push_back(innerVert);

        // Vertice sul raggio esterno
        SDL_Vertex outerVert;
        outerVert.position.x = static_cast<float>(centerX + outerRadius * std::cos(currentAngle));
        outerVert.position.y = static_cast<float>(centerY + outerRadius * std::sin(currentAngle));
        outerVert.color = {r, g, b, a};
        outerVert.tex_coord = {0.0f, 0.0f};
        vertices.push_back(outerVert);
    }

    // Aggiungi gli indici per formare i triangoli
    for (int i = 0; i < numArcSegments; ++i) {
        int v0 = i * 2;     // Vertice interno corrente
        int v1 = i * 2 + 1; // Vertice esterno corrente
        int v2 = (i + 1) * 2; // Vertice interno successivo
        int v3 = (i + 1) * 2 + 1; // Vertice esterno successivo

        // Primo triangolo del segmento (formato da interno corrente, esterno corrente, interno successivo)
        indices.push_back(v0);
        indices.push_back(v1);
        indices.push_back(v2);

        // Secondo triangolo del segmento (formato da interno successivo, esterno corrente, esterno successivo)
        indices.push_back(v2);
        indices.push_back(v1);
        indices.push_back(v3);
    }

    // Esegui una singola chiamata a SDL_RenderGeometry
    SDL_RenderGeometry(renderer, nullptr,
                       vertices.data(), static_cast<int>(vertices.size()),
                       indices.data(), static_cast<int>(indices.size()));
}

void Draw::present() {
    SDL_RenderPresent(window.getSDLRenderer());
}