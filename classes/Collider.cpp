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
    float entityX = currentGalaxy.positions[entity].x + currentGalaxy.velocities[entity].x;
    float entityY = currentGalaxy.positions[entity].y + currentGalaxy.velocities[entity].y;
    float planetX = currentGalaxy.positions[planet].x;
    float planetY = currentGalaxy.positions[planet].y;

    float headX = entityX + std::cos(currentGalaxy.angles[entity].deg * M_PI / 180.0f - M_PI / 2) * currentGalaxy.sizes[entity].height;
    float headY = entityY + std::sin(currentGalaxy.angles[entity].deg * M_PI / 180.0f - M_PI / 2) * currentGalaxy.sizes[entity].height;
    // Check if the entity's head is in the planet
    bool headInPlanet = pointInCircle(
        headX + currentGalaxy.velocities[entity].x , headY + currentGalaxy.velocities[entity].y,
        planetX, planetY,
        currentGalaxy.radii[planet].value + 5
    );
    //xor for layers
    for(size_t i = 0; i < currentGalaxy.layers[planet].size(); i++) {
        float layerValue = currentGalaxy.layers[planet][i].value;
        headInPlanet ^= pointInCircle(
            headX + currentGalaxy.velocities[entity].x,
            headY + currentGalaxy.velocities[entity].y,
            planetX,
            planetY,
            layerValue + 5
        );
    }
    // Check if the entity's head is in any entry
    for(size_t i = 0; i < currentGalaxy.layerEntries[planet].size(); i++) {
        size_t entryIndex = currentGalaxy.layerEntries[planet][i];
        float innerR = currentGalaxy.layers[planet][currentGalaxy.startLayers[entryIndex]].value;
        float outerR;
        if(currentGalaxy.startLayers[entryIndex] == 0){
            outerR = currentGalaxy.radii[planet].value;
        } else {
            outerR = currentGalaxy.layers[planet][currentGalaxy.startLayers[entryIndex] - 1].value;
        }
        if(pointInWall(
            headX + currentGalaxy.velocities[entity].x,
            headY + currentGalaxy.velocities[entity].y,
            currentGalaxy.positions[planet].x,
            currentGalaxy.positions[planet].y,
            innerR - 5, outerR + 5,
            currentGalaxy.widths[entryIndex],
            currentGalaxy.angles[entryIndex].deg
        )) {
            headInPlanet = false;
        }
    }

    if(headInPlanet){
        currentGalaxy.velocities[entity] = subtract(currentGalaxy.velocities[entity], velocityTowardsPoint(currentGalaxy.positions[planet], currentGalaxy.positions[entity], currentGalaxy.velocities[entity]));
        return false;
    }

    // Check if the entity's body is in the planet
    inPlanet = pointInCircle(
        entityX, entityY,
        planetX, planetY,
        currentGalaxy.radii[planet].value + 5
    );
    //xor for layers
    for(size_t i = 0; i < currentGalaxy.layers[planet].size(); i++) {
        float layerValue = currentGalaxy.layers[planet][i].value;
        inPlanet ^= pointInCircle(
            entityX,
            entityY,
            planetX,
            planetY,
            layerValue + 5
        );
    }
    //is in entry
    for(size_t i = 0; i < currentGalaxy.layerEntries[planet].size(); i++) {
        size_t entryIndex = currentGalaxy.layerEntries[planet][i];
        float innerR = currentGalaxy.layers[planet][currentGalaxy.startLayers[entryIndex]].value;
        float outerR;
        if(currentGalaxy.startLayers[entryIndex] == 0){
            outerR = currentGalaxy.radii[planet].value;
        } else {
            outerR = currentGalaxy.layers[planet][currentGalaxy.startLayers[entryIndex] - 1].value;
        }
        if(pointInWall(
            entityX,
            entityY,
            currentGalaxy.positions[planet].x,
            currentGalaxy.positions[planet].y,
            innerR - 5, outerR + 5,
            currentGalaxy.widths[entryIndex],
            currentGalaxy.angles[entryIndex].deg
        )) {
            return false;
        }
    }
    //is in wall
    for(size_t i = 0; i < currentGalaxy.layerWalls[planet].size(); i++) {
        size_t wallIndex = currentGalaxy.layerWalls[planet][i];
        if( pointInWall(
            entityX,
            entityY,
            currentGalaxy.positions[planet].x,
            currentGalaxy.positions[planet].y,
            currentGalaxy.layers[planet][currentGalaxy.startLayers[wallIndex]].value - 5,
            currentGalaxy.layers[planet][currentGalaxy.endLayers[wallIndex]].value + 5,
            currentGalaxy.widths[wallIndex],
            currentGalaxy.angles[wallIndex].deg
        )) {
            currentGalaxy.wallIndexes[entity] = wallIndex;
            return false;
        }
    }
    return inPlanet;
}

bool Collider::wormInPlanet(size_t planet, size_t wormIndex, size_t node, Galaxy& currentGalaxy){
    bool inPlanet = false;
    Vec2 position = currentGalaxy.wormPositions[wormIndex][node];
    Vec2 planetPosition = currentGalaxy.positions[planet];
    inPlanet = pointInCircle(position.x, position.y, planetPosition.x, planetPosition.y, currentGalaxy.radii[planet].value + 5);
    //xor for layers
    for(size_t i = 0; i < currentGalaxy.layers[planet].size(); i++) {
        float layerValue = currentGalaxy.layers[planet][i].value;
        inPlanet ^= pointInCircle(
            position.x,
            position.y,
            planetPosition.x,
            planetPosition.y,
            layerValue + 5
        );
    }
    //entries
    for(size_t i = 0; i < currentGalaxy.layerEntries[planet].size(); i++) {
        size_t entryIndex = currentGalaxy.layerEntries[planet][i];
        float innerR = currentGalaxy.layers[planet][currentGalaxy.startLayers[entryIndex]].value;
        float outerR;
        if(currentGalaxy.startLayers[entryIndex] == 0){
            outerR = currentGalaxy.radii[planet].value;
        } else {
            outerR = currentGalaxy.layers[planet][currentGalaxy.startLayers[entryIndex] - 1].value;
        }
        if(pointInWall(
            position.x,
            position.y,
            currentGalaxy.positions[planet].x,
            currentGalaxy.positions[planet].y,
            innerR - 5, outerR + 5,
            currentGalaxy.widths[entryIndex],
            currentGalaxy.angles[entryIndex].deg
        )) {
            return false;
        }
    }
    return inPlanet;
}

void Collider::entitiesPlatforms(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.entities.size(); i++) {
        size_t entityIndex = currentGalaxy.entities[i].index;
        float px = currentGalaxy.positions[entityIndex].x;
        float py = currentGalaxy.positions[entityIndex].y;
        currentGalaxy.platformIndexes[entityIndex] = -1;

        for (size_t j = 0; j < currentGalaxy.platforms.size(); j++) {
            size_t platformIndex = currentGalaxy.platforms[j].index;

            int planetId = currentGalaxy.planetIndexes[platformIndex];
            if (planetId < 0) continue;

            float cx = currentGalaxy.positions[planetId].x;
            float cy = currentGalaxy.positions[planetId].y;
            float radius = currentGalaxy.radii[planetId].value;

            float height = currentGalaxy.sizes[platformIndex].height;
            float width = currentGalaxy.sizes[platformIndex].width;
            float angle = currentGalaxy.angles[platformIndex].deg;

            if (pointInPlatform(px, py, cx, cy, radius, height + 5, width, angle)) {
                currentGalaxy.platformIndexes[entityIndex] = platformIndex;
                currentGalaxy.planetIndexes[entityIndex] = currentGalaxy.planetIndexes[platformIndex];
                continue;
            }
        }
    }
}

void Collider::entitiesPlanets(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.entities.size(); i++) {
        size_t entityIndex = currentGalaxy.entities[i].index;
        float px = currentGalaxy.positions[entityIndex].x;
        float py = currentGalaxy.positions[entityIndex].y;

        int currentPlanetId = currentGalaxy.planetIndexes[entityIndex];

        currentGalaxy.planetIndexes[entityIndex] = -1;
        currentGalaxy.wallIndexes[entityIndex] = -1;

        for (size_t j = 0; j < currentGalaxy.planets.size(); j++) {
            size_t planetIndex = currentGalaxy.planets[j].index;
            if(pointInPlanet(planetIndex, entityIndex, currentGalaxy)) {
                currentGalaxy.planetIndexes[entityIndex] = planetIndex;
            }
        }
    }
}

void Collider::wormsPlanets(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.worms.size(); i++) {
        size_t wormIndex = currentGalaxy.worms[i].index;
        for (size_t j = 0; j < currentGalaxy.wormPositions[wormIndex].size(); j++) {
            currentGalaxy.wormPlanetIndexes[wormIndex][j] = -1;
            int currentPlanetId = -1;
            for (size_t k = 0; k < currentGalaxy.planets.size(); k++) {
                size_t planetIndex = currentGalaxy.planets[k].index;
                if (wormInPlanet(planetIndex, wormIndex, j, currentGalaxy)) {
                    currentPlanetId = planetIndex;
                    currentGalaxy.wormPlanetIndexes[wormIndex][j] = planetIndex;
                }
            }
        }
    }
}

void Collider::updateCollisions(Galaxy& currentGalaxy) {
    entitiesPlanets(currentGalaxy);
    entitiesPlatforms(currentGalaxy);
    //wormsPlanets(currentGalaxy);
}