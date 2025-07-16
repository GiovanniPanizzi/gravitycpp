#include "../../../include/classes/generalPurpose/Draw.hpp"
#include <iostream>
#include <functional>
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

void Draw::drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Renderer* renderer = window.getSDLRenderer();
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_Rect rect = { x, y, w, h };
    SDL_RenderFillRect(renderer, &rect);
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
    if (numArcSegments < 5) numArcSegments = 5; // Minimo 2 segmenti per un inizio e fine

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

void Draw::drawAnnularSectionWithPalette(
    int centerX, int centerY, 
    int innerRadius, int outerRadius, 
    float startAngleRad, float endAngleRad, 
    const std::vector<SDL_Color>& palette,
    Uint8 a = 255)
{
    SDL_Renderer* renderer = window.getSDLRenderer();

    // Calculate number of segments (blocks) along the arc
    int segments = std::max(4, int((endAngleRad - startAngleRad) / (2 * M_PI) * 300));
    float totalAngle = endAngleRad - startAngleRad;
    int numArcSegments = static_cast<int>(std::ceil(totalAngle / (2 * M_PI) * segments * (outerRadius - innerRadius)));
    if (numArcSegments < 2) numArcSegments = 2;

    std::vector<SDL_Vertex> vertices;
    std::vector<int> indices;

    vertices.reserve((numArcSegments + 1) * 2);
    indices.reserve(numArcSegments * 6);

    for (int i = 0; i <= numArcSegments; ++i) {
        float currentAngle = startAngleRad + (static_cast<float>(i) / numArcSegments) * totalAngle;

        // Choose color from palette cycling by segment index (except for last vertex)
        // For the last vertex (i == numArcSegments) reuse previous color to avoid mismatch
        int colorIndex = (i == numArcSegments) ? (i - 1) % static_cast<int>(palette.size()) : i % static_cast<int>(palette.size());
        SDL_Color col = palette[colorIndex];
        col.a = a;

        // Inner vertex
        SDL_Vertex innerVert;
        innerVert.position.x = static_cast<float>(centerX + innerRadius * std::cos(currentAngle));
        innerVert.position.y = static_cast<float>(centerY + innerRadius * std::sin(currentAngle));
        innerVert.color = col;
        innerVert.tex_coord = {0.0f, 0.0f};
        vertices.push_back(innerVert);

        // Outer vertex
        SDL_Vertex outerVert;
        outerVert.position.x = static_cast<float>(centerX + outerRadius * std::cos(currentAngle));
        outerVert.position.y = static_cast<float>(centerY + outerRadius * std::sin(currentAngle));
        outerVert.color = col;
        outerVert.tex_coord = {0.0f, 0.0f};
        vertices.push_back(outerVert);
    }

    // Create indices for triangles forming quads between segments
    for (int i = 0; i < numArcSegments; ++i) {
        int v0 = i * 2;       // inner current
        int v1 = i * 2 + 1;   // outer current
        int v2 = (i + 1) * 2; // inner next
        int v3 = (i + 1) * 2 + 1; // outer next

        // First triangle
        indices.push_back(v0);
        indices.push_back(v1);
        indices.push_back(v2);

        // Second triangle
        indices.push_back(v2);
        indices.push_back(v1);
        indices.push_back(v3);
    }

    SDL_RenderGeometry(renderer, nullptr,
                       vertices.data(), static_cast<int>(vertices.size()),
                       indices.data(), static_cast<int>(indices.size()));
}

void Draw::adjustCameraPosition(Galaxy& currentGalaxy) {
    Vec2 playerPosition = currentGalaxy.humans.positions[0];

    float offsetDistance = 50.0f;
    float direction = currentGalaxy.humans.directions[0].value;
    float angleRad = currentGalaxy.humans.angles[0].rad;

    float offsetX = offsetDistance * direction * cos(angleRad);
    float offsetY = offsetDistance * direction * sin(angleRad);

    float targetX = playerPosition.x + offsetX;
    float targetY = playerPosition.y + offsetY;

    float dist = distance(Vec2{targetX, targetY}, cameraPosition);

    float t = 0.01f;

    cameraPosition.x += (targetX - cameraPosition.x) * t * dist / 30.0f;
    cameraPosition.y += (targetY - cameraPosition.y) * t * dist / 30.0f;
    
    float targetAngle = currentGalaxy.humans.angles[0].rad;
    float angleDiff = targetAngle - cameraAngle;

    // Porta angleDiff in [-π, π]
    if (angleDiff > M_PI) {
        angleDiff -= 2 * M_PI;
    } else if (angleDiff < -M_PI) {
        angleDiff += 2 * M_PI;
    }

    // Ignora piccole variazioni (anti-scattino)
    if (fabs(angleDiff) < 0.01f) angleDiff = 0.0f;

    // Applica un easing leggero (più fluido)
    float easingFactor = 30.0f * (fabs(angleDiff) / M_PI); // più basso = più smorzato
    float baseDivisor = (currentGalaxy.humans.planetIndexes[0] == -1) ? 500.0f : 120.0f;
    float interp = t * dist / baseDivisor;

    // Limita interpolazione e smorza con easingFactor
    interp = std::min(interp, 0.001f); // previene salti
    cameraAngle += angleDiff * interp * easingFactor;

    // Normalizza cameraAngle in [0, 2π]
    cameraAngle = fmod(cameraAngle, 2 * M_PI);
    if (cameraAngle < 0) cameraAngle += 2 * M_PI;
}

void Draw::drawPlanet(Galaxy& currentGalaxy, size_t i) {

    SDL_Renderer* renderer = window.getSDLRenderer();

    // Generate a color palette based on a base RGB color with small random variations
    auto generatePalette = [](Uint8 baseR, Uint8 baseG, Uint8 baseB, int numColors = 10, int variation = 2) {
        std::vector<SDL_Color> palette;
        auto clampColor = [](int val) -> Uint8 {
            return static_cast<Uint8>(std::clamp(val, 0, 255));
        };
        // Add base color as first palette color
        palette.push_back({ baseR, baseG, baseB, 255});
        // Generate additional colors with slight random variation
        for (int i = 1; i < numColors; ++i) {
            int vr = (rand() % (variation * 2 + 1)) - variation;
            int vg = (rand() % (variation * 2 + 1)) - variation;
            int vb = (rand() % (variation * 2 + 1)) - variation;
            palette.push_back({
                clampColor(baseR + vr),
                clampColor(baseG + vg),
                clampColor(baseB + vb),
                255
            });
        }
        return palette;
    };

    // Draw a planet as a circle composed of small colored blocks (pixels)
    auto drawPlanetWithBlocks = [&](int centerX, int centerY, float radius, const std::vector<SDL_Color>& palette) {
        int blockSize = 5; // Size of each block
        for (int y = -static_cast<int>(radius); y <= static_cast<int>(radius); y += blockSize) {
            for (int x = -static_cast<int>(radius); x <= static_cast<int>(radius); x += blockSize) {
                float dist = std::sqrt(float(x*x + y*y));
                if (dist <= radius) {
                    // Choose color from palette in a pattern based on block position
                    int index = (x / blockSize + y / blockSize) % static_cast<int>(palette.size());
                    if (index < 0) index += palette.size();
                    SDL_Color c = palette[index];
                    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
                    SDL_FRect rect = { static_cast<float>(centerX + x), static_cast<float>(centerY + y), (float)blockSize, (float)blockSize };
                    SDL_RenderFillRectF(renderer, &rect);
                }
            }
        }
    };

    if (currentGalaxy.planets.textures[i] != nullptr) {
        SDL_Texture* texture = currentGalaxy.planets.textures[i];
        Vec2 position = currentGalaxy.planets.positions[i];
        Radius radius = currentGalaxy.planets.radii[i];
        Angle angle = currentGalaxy.planets.angles[i];

        // Compute position relative to camera
        Vec2 relativePos = subtract(position, cameraPosition);

        // Rotate around camera by -cameraAngle
        float cosA = cos(-cameraAngle);
        float sinA = sin(-cameraAngle);

        float rotatedX = relativePos.x * cosA - relativePos.y * sinA;
        float rotatedY = relativePos.x * sinA + relativePos.y * cosA;

        // Screen coordinates with scale and screen center offset
        int x = static_cast<int>(rotatedX * scale + screenWidth / 2);
        int y = static_cast<int>(rotatedY * scale + screenHeight / 1.5f);

        int pivotX = static_cast<int>(radius.value * scale);
        int pivotY = static_cast<int>(radius.value * scale);

        int w = static_cast<int>(radius.value * scale * 2);
        int h = static_cast<int>(radius.value * scale * 2);

        SDL_FRect dstRect;
        dstRect.x = x - pivotX;
        dstRect.y = y - pivotY;
        dstRect.w = static_cast<float>(w);
        dstRect.h = static_cast<float>(h);

        SDL_FPoint pivot = { static_cast<float>(pivotX), static_cast<float>(pivotY) };

        // Rotate planet by its angle minus camera angle (in degrees)
        float angleDeg = static_cast<float>((angle.rad - cameraAngle) * 180.0 / M_PI);

        SDL_RenderCopyExF(renderer, texture, NULL, &dstRect, angleDeg, &pivot, SDL_FLIP_NONE);
    }
    else {
        int diameter = static_cast<int>(currentGalaxy.planets.radii[i].value * 2.0f * scale);
        SDL_Texture* texture = SDL_CreateTexture(renderer,
                                                SDL_PIXELFORMAT_RGBA8888,
                                                SDL_TEXTUREACCESS_TARGET,
                                                diameter, diameter);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        if (!texture) {
            SDL_Log("Error creating texture: %s", SDL_GetError());
            return;
        }
        SDL_SetRenderTarget(renderer, texture);
        SDL_RenderSetViewport(renderer, nullptr);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        int centerX = diameter / 2;
        int centerY = diameter / 2;
        float radius = currentGalaxy.planets.radii[i].value * scale;

        // Base color for planet surface
        Uint8 baseR = 60, baseG = 40, baseB = 40;
        if (!currentGalaxy.planets.layers[i].empty()) {
            switch (currentGalaxy.planets.layers[i][0].material) {
                case Material::ROCK: baseR = 60; baseG = 40; baseB = 40; break;
                case Material::ICE: baseR = 100; baseG = 100; baseB = 255; break;
                case Material::METAL: baseR = 200; baseG = 200; baseB = 200; break;
                case Material::GRAVITANIUM: baseR = 50; baseG = 0; baseB = 100; break;
                case Material::VOID: baseR = 50; baseG = 20; baseB = 20; break;
                default: baseR = 255; baseG = 255; baseB = 255; break;
            }
        }

        auto basePalette = generatePalette(baseR, baseG, baseB, 2, 16);  // simpler palette, no variance
        drawPlanetWithBlocks(centerX, centerY, radius, basePalette);

        // Draw layers simply with solid color blocks (no patterns)
        for (const auto& section : currentGalaxy.planets.layers[i]) {
            float outerRadius = section.shape.outerRadius.value * scale;
            float innerRadius = section.shape.innerRadius.value * scale;
            float startAngle = section.shape.startAngle.rad;
            float endAngle = section.shape.endAngle.rad;

            Uint8 r = 255, g = 255, b = 255;
            switch (section.material) {
                case Material::ROCK: r = 60; g = 40; b = 40; break;
                case Material::ICE: r = 100; g = 100; b = 255; break;
                case Material::METAL: r = 200; g = 200; b = 200; break;
                case Material::GRAVITANIUM: r = 50; g = 0; b = 100; break;
                case Material::VOID: r = 50; g = 20; b = 20; break;
                default: r = 255; g = 255; b = 255; break;
            }

            // Create palette for layer with colors slightly brighter than base
            auto layerPalette = generatePalette(std::min(r + 30, 255), std::min(g + 30, 255), std::min(b + 30, 255), 4, 3);

            // Draw the annular section ring with palette colors
            drawAnnularSectionWithPalette(centerX, centerY,
                                        static_cast<int>(innerRadius),
                                        static_cast<int>(outerRadius),
                                        startAngle, endAngle,
                                        layerPalette);
        }

        SDL_SetRenderTarget(renderer, nullptr);
        currentGalaxy.planets.textures[i] = texture;
    }
}

void Draw::drawPlanetPlatform(Galaxy& currentGalaxy, size_t i){
    SDL_Renderer* renderer = window.getSDLRenderer();
     size_t planet = currentGalaxy.platforms.planetIndexes[i];
        Vec2 position = currentGalaxy.planets.positions[planet];
        Vec2 relativePos = subtract(position, cameraPosition);
        AnnularSection platformShape = currentGalaxy.platforms.shapes[i];

        // Rotate around camera by -cameraAngle
        float cosA = cos(-cameraAngle);
        float sinA = sin(-cameraAngle);

        float rotatedX = relativePos.x * cosA - relativePos.y * sinA;
        float rotatedY = relativePos.x * sinA + relativePos.y * cosA;

        // Screen coordinates with scale and screen center offset
        int x = static_cast<int>(rotatedX * scale + screenWidth / 2);
        int y = static_cast<int>(rotatedY * scale + screenHeight / 1.5f);

        drawAnnularSection(
            x, y,
            static_cast<int>(platformShape.innerRadius.value * scale),
            static_cast<int>(platformShape.outerRadius.value * scale),
            platformShape.startAngle.rad - cameraAngle,
            platformShape.endAngle.rad - cameraAngle,
            255, 255, 255, 255
        );
}

void Draw::drawGalaxy(Galaxy& currentGalaxy){
    SDL_Renderer* renderer = window.getSDLRenderer();

    int cellSize = 32;
    int starDensity = 80;
    float parallax = 0.2f;

    Vec2 starCamPos = {
        cameraPosition.x * parallax,
        cameraPosition.y * parallax
    };

    Vec2 screenCenter = {
        static_cast<float>(screenWidth) / 2.0f,
        static_cast<float>(screenHeight) / 1.5f
    };
    float rangeX = screenWidth * 1.5f / scale;
    float rangeY = screenHeight * 1.5f / scale;

    int minX = static_cast<int>(floor((starCamPos.x - rangeX) / cellSize));
    int maxX = static_cast<int>(ceil((starCamPos.x + rangeX) / cellSize));
    int minY = static_cast<int>(floor((starCamPos.y - rangeY) / cellSize));
    int maxY = static_cast<int>(ceil((starCamPos.y + rangeY) / cellSize));

    for (int gx = minX; gx <= maxX; gx++) {
        for (int gy = minY; gy <= maxY; gy++) {
            float worldX = gx * cellSize * scale;
            float worldY = gy * cellSize * scale;

            // Hash deterministico
            int seed = (gx * 73856093) ^ (gy * 19349663) ^ (currentGalaxy.starsDistribution * 83492791);
            seed = seed & 0x7fffffff;

            if (seed % starDensity == 0) {
                Vec2 local = {
                    worldX - starCamPos.x,
                    worldY - starCamPos.y
                };

                Vec2 rotated = rotatePoint(local, {0, 0}, -cameraAngle);

                Vec2 screenPos = {
                    rotated.x + screenCenter.x,
                    rotated.y + screenCenter.y
                };

                if (screenPos.x >= -10 && screenPos.x < screenWidth + 10 &&
                    screenPos.y >= -10 && screenPos.y < screenHeight + 10) {

                    int sizeSeed = (seed / 100) % 5; 
                    int radius = 1;
                    Uint8 r = 255, g = 255, b = 255;

                    switch(sizeSeed) {
                        case 0: radius = 1; break;
                        case 1: radius = 1; r = 200; g = 200; b = 255; break;
                        case 2: radius = 2; r = 255; g = 240; b = 200; break;
                        case 3: radius = 2; r = 180; g = 255; b = 255; break;
                        case 4: radius = 3; r = 255; g = 255; b = 255; break;
                    }

                    radius = static_cast<int>(radius * scale);
                    if(radius < 1) radius = 1; 

                    drawFilledCircle(screenPos.x, screenPos.y, radius, r, g, b, 255);
                }
            }
        }
    }

    // Center of the screen (camera)
    float cx = screenWidth / 2.0f;
    float cy = screenHeight / 2.0f;

    // Loop over all planets
    for (int i = 0; i < currentGalaxy.planets.entities.size(); i++) {
        drawPlanet(currentGalaxy, i);
    }

    // draw platforms
    for (int i = 0; i < currentGalaxy.platforms.entities.size(); i++) {
        if (currentGalaxy.platforms.planetIndexes[i] != -1) {
            drawPlanetPlatform(currentGalaxy, i);
        }
    }

    // Draw humans, applying the same rotation relative to the camera
    for (int i = 0; i < currentGalaxy.humans.positions.size(); i++) {
        Vec2 position = currentGalaxy.humans.positions[i];
        RectSize size = currentGalaxy.humans.sizes[i];
        Angle angle = currentGalaxy.humans.angles[i];

        Vec2 relativePos = subtract(position, cameraPosition);

        float cosA = cos(-cameraAngle);
        float sinA = sin(-cameraAngle);

        float rotatedX = relativePos.x * cosA - relativePos.y * sinA;
        float rotatedY = relativePos.x * sinA + relativePos.y * cosA;

        int x = static_cast<int>(rotatedX * scale - size.width * scale / 2 + screenWidth / 2);
        int y = static_cast<int>(rotatedY * scale - size.height * scale + screenHeight / 1.5f);

        float objAngleDeg = (angle.rad - cameraAngle) * 180.0f / M_PI;

        drawFilledRotatedRect(x, y, static_cast<int>(size.width * scale), static_cast<int>(size.height * scale), objAngleDeg, size.width * scale / 2, size.height * scale, 100, 50, 130, 255);
    }
}

void Draw::present() {
    SDL_RenderPresent(window.getSDLRenderer());
}