#include "../headers/WormController.h"

int32_t value = 0;

void WormController::moveLeft(Galaxy& currentGalaxy, size_t wormIndex) {
    if (currentGalaxy.worms.planetIndexes[wormIndex][0] != -1) {
        int planetIndex = currentGalaxy.worms.planetIndexes[wormIndex][0];
        Vec2& wormHeadPos = currentGalaxy.worms.positions[wormIndex][0];
        Vec2& planetPos = currentGalaxy.planets.positions[planetIndex];

        float dx = wormHeadPos.x - planetPos.x;
        float dy = wormHeadPos.y - planetPos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        currentGalaxy.worms.velocities[wormIndex][0].x += dy * 0.5f / dist;
        currentGalaxy.worms.velocities[wormIndex][0].y -= dx * 0.5f / dist;
    }
}

void WormController::moveRight(Galaxy& currentGalaxy, size_t wormIndex) {
    if (currentGalaxy.worms.planetIndexes[wormIndex][0] != -1) {
        int planetIndex = currentGalaxy.worms.planetIndexes[wormIndex][0];
        Vec2& wormHeadPos = currentGalaxy.worms.positions[wormIndex][0];
        Vec2& planetPos = currentGalaxy.planets.positions[planetIndex];

        float dx = wormHeadPos.x - planetPos.x;
        float dy = wormHeadPos.y - planetPos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        currentGalaxy.worms.velocities[wormIndex][0].x -= dy * 0.5f / dist;
        currentGalaxy.worms.velocities[wormIndex][0].y += dx * 0.5f / dist;
    }
}

void WormController::allMove(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.worms.entities.size(); i++) {
        size_t wormIndex = currentGalaxy.worms.entities[i].index;
        moveRight(currentGalaxy, wormIndex);
        value++;
    }
}