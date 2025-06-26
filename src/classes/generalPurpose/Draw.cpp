#include "../../../include/classes/generalPurpose/Draw.hpp"
#include <iostream>
#include <vector>

Draw::Draw(Window& window) : window(window) {}

Draw::~Draw() {}

void Draw::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(window.getSDLRenderer(), r, g, b, a);
    SDL_RenderClear(window.getSDLRenderer());
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

void Draw::drawAnnularSection(int centerX, int centerY, int innerRadius, int outerRadius, float startAngleRad, float endAngleRad, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
 SDL_Renderer* renderer = window.getSDLRenderer();

    int segments = std::max(4, int((endAngleRad - startAngleRad) / (2 * M_PI) * 300));
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

void Draw::adjustCameraPosition(Galaxy& currentGalaxy) {
    Vec2 playerPosition = currentGalaxy.humans.positions[0];

    float offsetDistance = 50.0f;
    float direction = currentGalaxy.humans.directions[0].value;
    float angleRad = currentGalaxy.humans.angles[0].rad * M_PI / 180.0f;

    float offsetX = offsetDistance * direction * cos(angleRad);
    float offsetY = offsetDistance * direction * sin(angleRad);

    float targetX = playerPosition.x + offsetX;
    float targetY = playerPosition.y + offsetY;

    float dist = distance(Vec2{targetX, targetY}, cameraPosition);

    float t = 0.01f;

    cameraPosition.x += (targetX - cameraPosition.x) * t * dist / 30.0f;
    cameraPosition.y += (targetY - cameraPosition.y) * t * dist / 30.0f;
}

void Draw::drawGalaxy(Galaxy& currentGalaxy){
    SDL_Renderer* renderer = window.getSDLRenderer();

    //draw planets
    for (int i = 0; i < currentGalaxy.planets.entities.size(); i++) {

        if(currentGalaxy.planets.textures[i] != nullptr) {
            SDL_Texture* texture = currentGalaxy.planets.textures[i];
            Vec2 position = currentGalaxy.planets.positions[i];
            Radius radius = currentGalaxy.planets.radii[i];
            Angle angle = currentGalaxy.planets.angles[i];

            int x = static_cast<int>(position.x * scale - cameraPosition.x * scale + screenWidth / 2);
            int y = static_cast<int>(position.y * scale - cameraPosition.y * scale + screenHeight / 2);

            int pivotX = static_cast<int>(radius.value * scale);
            int pivotY = static_cast<int>(radius.value * scale);

            double angleRad = angle.rad * M_PI / 180.0;

            int w = static_cast<int>(radius.value * scale * 2);
            int h = static_cast<int>(radius.value * scale * 2);

            SDL_FRect dstRect;
            dstRect.x = x - pivotX;
            dstRect.y = y - pivotY;
            dstRect.w = static_cast<float>(w);
            dstRect.h = static_cast<float>(h);

            SDL_FPoint pivot = { static_cast<float>(pivotX), static_cast<float>(pivotY) };

            float angleDeg = static_cast<float>(angleRad * 180.0 / M_PI);

            SDL_RenderCopyExF(window.getSDLRenderer(), texture, NULL, &dstRect, angleDeg, &pivot, SDL_FLIP_NONE);
        }

        //create texture for planets
        if (currentGalaxy.planets.textures[i] == nullptr) {

            int diameter = static_cast<int>(currentGalaxy.planets.radii[i].value * 2.0f * scale);

            SDL_Texture* texture = SDL_CreateTexture(renderer,
                                                    SDL_PIXELFORMAT_RGBA8888,
                                                    SDL_TEXTUREACCESS_TARGET,
                                                    diameter, diameter);
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            if (!texture) {
                SDL_Log("Errore creazione texture: %s", SDL_GetError());
                continue;
            }

            SDL_SetRenderTarget(renderer, texture);
            SDL_RenderSetViewport(renderer, nullptr);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);

            int centerX = diameter / 2;
            int centerY = diameter / 2;

            if(currentGalaxy.planets.layers[i].empty()) {
                drawFilledCircle(centerX, centerY, static_cast<int>(currentGalaxy.planets.radii[i].value * scale), 60, 40, 40, 255);
                SDL_SetRenderTarget(renderer, nullptr);
                currentGalaxy.planets.textures[i] = texture;
                continue;
            }

            drawFilledCircle(centerX, centerY, static_cast<int>(currentGalaxy.planets.radii[i].value * scale), 40, 20, 20, 255);

            for (const auto& section : currentGalaxy.planets.layers[i]) {
                float outerRadius = section.shape.outerRadius.value * scale;
                float innerRadius = section.shape.innerRadius.value * scale;
                float startAngle = section.shape.startAngle.rad;
                float endAngle = section.shape.endAngle.rad;

                Uint8 r = 255, g = 255, b = 255, a = 255;
                switch (section.material) {
                    case Material::ROCK: r = 60; g = 40; b = 40; break;
                    case Material::ICE: r = 100; g = 100; b = 255; break;
                    case Material::METAL: r = 200; g = 200; b = 200; break;
                    case Material::GRAVITANIUM: r = 50; g = 0; b = 100; break;
                    case Material::VOID: r = 50; g = 20; b = 20; break;
                    default: r = 255; g = 255; b = 255; break;
                }

                SDL_SetRenderDrawColor(renderer, r, g, b, a);

                // Disegna la sezione circolare (approssimazione con triangoli)
                const int segments = 100;
                float step = (endAngle - startAngle) / segments;

                for (int s = 0; s < segments; ++s) {
                    float a1 = startAngle + s * step;
                    float a2 = startAngle + (s + 1) * step;

                    float x1_outer = centerX + outerRadius * std::cos(a1);
                    float y1_outer = centerY + outerRadius * std::sin(a1);
                    float x2_outer = centerX + outerRadius * std::cos(a2);
                    float y2_outer = centerY + outerRadius * std::sin(a2);

                    float x1_inner = centerX + innerRadius * std::cos(a1);
                    float y1_inner = centerY + innerRadius * std::sin(a1);
                    float x2_inner = centerX + innerRadius * std::cos(a2);
                    float y2_inner = centerY + innerRadius * std::sin(a2);

                    // Disegna il "quad" curvo come due triangoli
                    SDL_Vertex verts[6];

                    verts[0].position = { x1_inner, y1_inner };
                    verts[1].position = { x1_outer, y1_outer };
                    verts[2].position = { x2_outer, y2_outer };

                    verts[3].position = { x1_inner, y1_inner };
                    verts[4].position = { x2_outer, y2_outer };
                    verts[5].position = { x2_inner, y2_inner };

                    for (int v = 0; v < 6; ++v) {
                        verts[v].color.r = r;
                        verts[v].color.g = g;
                        verts[v].color.b = b;
                        verts[v].color.a = a;
                    }

                    SDL_RenderGeometry(renderer, nullptr, verts, 6, nullptr, 0);
                }
            }

            SDL_SetRenderTarget(renderer, nullptr);

            currentGalaxy.planets.textures[i] = texture;
            std::cout << "Texture creata per il pianeta " << i << std::endl;
        }
    }

    //draw humans
    for( int i = 0; i < currentGalaxy.humans.positions.size(); i++) {
        Vec2 position = currentGalaxy.humans.positions[i];
        RectSize size = currentGalaxy.humans.sizes[i];
        Angle angle = currentGalaxy.humans.angles[i];
        int x = static_cast<int>(position.x * scale - cameraPosition.x * scale - size.width / 2 + screenWidth / 2);
        int y = static_cast<int>(position.y * scale - cameraPosition.y * scale - size.height + screenHeight / 2);

        drawFilledRotatedRect(x, y, static_cast<int>(size.width * scale), static_cast<int>(size.height * scale), angle.rad * 180.0 / M_PI, size.width / 2, size.height, 30, 50, 90, 255);
    }
}

void Draw::present() {
    SDL_RenderPresent(window.getSDLRenderer());
}