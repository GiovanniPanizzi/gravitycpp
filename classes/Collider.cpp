#include "../headers/Collider.h"
#include <iostream>
#include <cmath>

void adjustVelocityRelativeToTrain(Vec2& entityVel, const Vec2& trainVel) {
    float dx = entityVel.x - trainVel.x;
    float dy = entityVel.y - trainVel.y;
    float mag = std::sqrt(dx*dx + dy*dy);
    const float EPSILON = 0.01f;

    if (mag >= EPSILON) {
        entityVel.x += trainVel.x;
        entityVel.y += trainVel.y;
    }
}

bool Collider::pointInCircle(float pointX, float pointY, float circleX, float circleY, float r) {
    float dx = pointX - circleX;
    float dy = pointY - circleY;
    return dx * dx + dy * dy <= r * r;
}

bool Collider::pointInPlatform(float pointX, float pointY,
                               float circleX, float circleY,
                               float planetR, float height,
                               float width, float angleDeg) {
    float dx = pointX - circleX;
    float dy = pointY - circleY;

    float dist = std::sqrt(dx * dx + dy * dy);
    float innerR = planetR;
    float outerR = planetR + height;

    if (dist > outerR) return false;
    if (dist < innerR) return false;

    float pointAngle = std::atan2(dy, dx) * 180.0f / M_PI;

    auto normalize = [](float deg) {
        while (deg < 0.0f) deg += 360.0f;
        while (deg >= 360.0f) deg -= 360.0f;
        return deg;
    };

    pointAngle = normalize(pointAngle);

    float startAngle = normalize(angleDeg - width / 2.0f);
    float endAngle = normalize(angleDeg + width / 2.0f);

    if (startAngle < endAngle) {
        return pointAngle >= startAngle && pointAngle <= endAngle;
    } else {
        return pointAngle >= startAngle || pointAngle <= endAngle;
    }
}

bool Collider::pointInWall(float pointX, float pointY, float circleX, float circleY, float innerR, float outerR, float width, float angleDeg) {
    float dx = pointX - circleX;
    float dy = pointY - circleY;

    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > outerR) return false;
    if (dist < innerR) return false;

    float pointAngle = std::atan2(dy, dx) * 180.0f / M_PI;

    auto normalize = [](float deg) {
        while (deg < 0.0f) deg += 360.0f;
        while (deg >= 360.0f) deg -= 360.0f;
        return deg;
    };

    pointAngle = normalize(pointAngle);

    float startAngle = normalize(angleDeg - width / 2.0f);
    float endAngle = normalize(angleDeg + width / 2.0f);

    if (startAngle < endAngle) {
        return pointAngle >= startAngle && pointAngle <= endAngle;
    } else {
        return pointAngle >= startAngle || pointAngle <= endAngle;
    }
}

bool Collider::pointInPlanet(size_t planet, size_t entity, Galaxy& currentGalaxy) {
    bool inPlanet = false;

    // Get entity and planet positions
    float entityX = currentGalaxy.humans.positions[entity].x + currentGalaxy.humans.velocities[entity].x;
    float entityY = currentGalaxy.humans.positions[entity].y + currentGalaxy.humans.velocities[entity].y;
    float planetX = currentGalaxy.planets.positions[planet].x;
    float planetY = currentGalaxy.planets.positions[planet].y;

    // Calculate the head position of the entity
    float headX = entityX + std::cos(currentGalaxy.humans.angles[entity].deg * M_PI / 180.0f - M_PI / 2) * currentGalaxy.humans.sizes[entity].height;
    float headY = entityY + std::sin(currentGalaxy.humans.angles[entity].deg * M_PI / 180.0f - M_PI / 2) * currentGalaxy.humans.sizes[entity].height;

    // Check if the entity's head is in the planet
    bool headInPlanet = pointInCircle(
        headX + currentGalaxy.humans.velocities[entity].x,
        headY + currentGalaxy.humans.velocities[entity].y,
        planetX, planetY,
        currentGalaxy.planets.radii[planet].value + 5
    );

    // XOR for layers
    for (size_t i = 0; i < currentGalaxy.planets.layers[planet].size(); i++) {
        float layerValue = currentGalaxy.planets.layers[planet][i].value;
        headInPlanet ^= pointInCircle(
            headX + currentGalaxy.humans.velocities[entity].x,
            headY + currentGalaxy.humans.velocities[entity].y,
            planetX,
            planetY,
            layerValue + 5
        );
    }

    // Check if the entity's head is in any entry
    for (size_t i = 0; i < currentGalaxy.planets.layerEntries[planet].size(); i++) {
        size_t entryIndex = currentGalaxy.planets.layerEntries[planet][i];
        float innerR = currentGalaxy.planets.layers[planet][currentGalaxy.entries.startLayers[entryIndex]].value;
        float outerR = (currentGalaxy.entries.startLayers[entryIndex] == 0)
            ? currentGalaxy.planets.radii[planet].value
            : currentGalaxy.planets.layers[planet][currentGalaxy.entries.startLayers[entryIndex] - 1].value;

        if (pointInWall(
            headX + currentGalaxy.humans.velocities[entity].x,
            headY + currentGalaxy.humans.velocities[entity].y,
            planetX, planetY,
            innerR - 5, outerR + 5,
            currentGalaxy.entries.widths[entryIndex],
            currentGalaxy.entries.angles[entryIndex].deg
        )) {
            headInPlanet = false;
        }
    }

    // If the head is in the planet, adjust velocity and return false
    if (headInPlanet) {
        currentGalaxy.humans.velocities[entity] = subtract(
            currentGalaxy.humans.velocities[entity],
            velocityTowardsPoint(
                currentGalaxy.planets.positions[planet],
                currentGalaxy.humans.positions[entity],
                currentGalaxy.humans.velocities[entity]
            )
        );
        return false;
    }

    // Check if the entity's body is in the planet
    inPlanet = pointInCircle(
        entityX, entityY,
        planetX, planetY,
        currentGalaxy.planets.radii[planet].value + 5
    );

    // XOR for layers
    for (size_t i = 0; i < currentGalaxy.planets.layers[planet].size(); i++) {
        float layerValue = currentGalaxy.planets.layers[planet][i].value;
        inPlanet ^= pointInCircle(
            entityX, entityY,
            planetX, planetY,
            layerValue + 5
        );
    }

    // Check if the entity's body is in any entry
    for (size_t i = 0; i < currentGalaxy.planets.layerEntries[planet].size(); i++) {
        size_t entryIndex = currentGalaxy.planets.layerEntries[planet][i];
        float innerR = currentGalaxy.planets.layers[planet][currentGalaxy.entries.startLayers[entryIndex]].value;
        float outerR = (currentGalaxy.entries.startLayers[entryIndex] == 0)
            ? currentGalaxy.planets.radii[planet].value
            : currentGalaxy.planets.layers[planet][currentGalaxy.entries.startLayers[entryIndex] - 1].value;

        if (pointInWall(
            entityX, entityY,
            planetX, planetY,
            innerR - 5, outerR + 5,
            currentGalaxy.entries.widths[entryIndex],
            currentGalaxy.entries.angles[entryIndex].deg
        )) {
            return false;
        }
    }

    // Check if the entity's body is in any wall
    for (size_t i = 0; i < currentGalaxy.planets.layerWalls[planet].size(); i++) {
        size_t wallIndex = currentGalaxy.planets.layerWalls[planet][i];
        if (pointInWall(
            entityX, entityY,
            planetX, planetY,
            currentGalaxy.planets.layers[planet][currentGalaxy.walls.startLayers[wallIndex]].value - 5,
            currentGalaxy.planets.layers[planet][currentGalaxy.walls.endLayers[wallIndex]].value + 5,
            currentGalaxy.walls.widths[wallIndex],
            currentGalaxy.walls.angles[wallIndex].deg
        )) {
            currentGalaxy.humans.wallIndexes[entity] = wallIndex;
            return false;
        }
    }

    return inPlanet;
}

bool Collider::wormInPlanet(size_t planet, size_t wormIndex, size_t node, Galaxy& currentGalaxy) {
    bool inPlanet = false;
    Vec2 position = currentGalaxy.worms.positions[wormIndex][node];
    Vec2 planetPosition = currentGalaxy.planets.positions[planet];

    // Check if the worm node is in the planet's radius
    inPlanet = pointInCircle(position.x, position.y, planetPosition.x, planetPosition.y, currentGalaxy.planets.radii[planet].value + 5);

    // XOR for layers
    for (size_t i = 0; i < currentGalaxy.planets.layers[planet].size(); i++) {
        float layerValue = currentGalaxy.planets.layers[planet][i].value;
        inPlanet ^= pointInCircle(
            position.x,
            position.y,
            planetPosition.x,
            planetPosition.y,
            layerValue + 5
        );
    }

    // Check entries
    for (size_t i = 0; i < currentGalaxy.planets.layerEntries[planet].size(); i++) {
        size_t entryIndex = currentGalaxy.planets.layerEntries[planet][i];
        float innerR = currentGalaxy.planets.layers[planet][currentGalaxy.entries.startLayers[entryIndex]].value;
        float outerR = (currentGalaxy.entries.startLayers[entryIndex] == 0)
            ? currentGalaxy.planets.radii[planet].value
            : currentGalaxy.planets.layers[planet][currentGalaxy.entries.startLayers[entryIndex] - 1].value;

        if (pointInWall(
            position.x,
            position.y,
            planetPosition.x,
            planetPosition.y,
            innerR - 5, outerR + 5,
            currentGalaxy.entries.widths[entryIndex],
            currentGalaxy.entries.angles[entryIndex].deg
        )) {
            return false;
        }
    }

    return inPlanet;
}

void Collider::entitiesPlatforms(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.humans.entities.size(); i++) {
        size_t entityIndex = currentGalaxy.humans.entities[i].index;
        float px = currentGalaxy.humans.positions[entityIndex].x;
        float py = currentGalaxy.humans.positions[entityIndex].y;

        currentGalaxy.humans.platformIndexes[entityIndex] = -1;

        for (size_t j = 0; j < currentGalaxy.planets.planetPlatforms.size(); j++) {
            for (size_t k = 0; k < currentGalaxy.planets.planetPlatforms[j].size(); k++) {
                size_t platformIndex = currentGalaxy.planets.planetPlatforms[j][k];
                int planetId = j;

                Vec2& planetPos = currentGalaxy.planets.positions[planetId];
                float radius = currentGalaxy.planets.radii[planetId].value;
                float height = currentGalaxy.platforms.sizes[platformIndex].height;
                float width = currentGalaxy.platforms.sizes[platformIndex].width;
                float angle = currentGalaxy.platforms.angles[platformIndex].deg;

                if (pointInPlatform(px, py, planetPos.x, planetPos.y, radius, height + 5, width, angle)) {
                    currentGalaxy.humans.platformIndexes[entityIndex] = platformIndex;
                    currentGalaxy.humans.planetIndexes[entityIndex] = planetId;
                    break;
                }
            }
        }
    }
}

void Collider::entitiesPlanets(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.humans.entities.size(); i++) {
        size_t entityIndex = currentGalaxy.humans.entities[i].index;
        float px = currentGalaxy.humans.positions[entityIndex].x;
        float py = currentGalaxy.humans.positions[entityIndex].y;

        currentGalaxy.humans.planetIndexes[entityIndex] = -1;
        currentGalaxy.humans.wallIndexes[entityIndex] = -1;

        for (size_t j = 0; j < currentGalaxy.planets.entities.size(); j++) {
            size_t planetIndex = currentGalaxy.planets.entities[j].index;

            if (pointInPlanet(planetIndex, entityIndex, currentGalaxy)) {
                currentGalaxy.humans.planetIndexes[entityIndex] = planetIndex;
            }
        }
    }
}

void Collider::wormsPlanets(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.worms.entities.size(); i++) {
        size_t wormIndex = currentGalaxy.worms.entities[i].index;

        for (size_t j = 0; j < currentGalaxy.worms.positions[wormIndex].size(); j++) {
            currentGalaxy.worms.planetIndexes[wormIndex][j] = -1;

            for (size_t k = 0; k < currentGalaxy.planets.entities.size(); k++) {
                size_t planetIndex = currentGalaxy.planets.entities[k].index;

                if (wormInPlanet(planetIndex, wormIndex, j, currentGalaxy)) {
                    currentGalaxy.worms.planetIndexes[wormIndex][j] = planetIndex;
                }
            }
        }
    }
}

void Collider::updateCollisions(Galaxy& currentGalaxy) {
    // Update collisions between entities and planets
    entitiesPlanets(currentGalaxy);

    // Update collisions between entities and platforms
    entitiesPlatforms(currentGalaxy);

    // Update collisions between worms and planets
    wormsPlanets(currentGalaxy);
}