#include "../headers/Collider.h"
#include <iostream>
#include <cmath>


void adjustVelocityRelativeToTrain(Velocity& entityVel, const Velocity& trainVel) {
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
        if(pointAngle >= startAngle && pointAngle <= endAngle)
        return pointAngle >= startAngle && pointAngle <= endAngle;
    } else {
        if(pointAngle >= startAngle || pointAngle <= endAngle)
        return pointAngle >= startAngle || pointAngle <= endAngle;
    }
}

bool Collider::pointInPlanet(size_t planet, size_t entity, Galaxy& currentGalaxy) {
    bool inPlanet = false;
    inPlanet = pointInCircle(
        currentGalaxy.positions[entity].x,
        currentGalaxy.positions[entity].y,
        currentGalaxy.positions[planet].x,
        currentGalaxy.positions[planet].y,
        currentGalaxy.radii[planet].value + 5
    );
    //xor for layers
    for(size_t i = 0; i < currentGalaxy.layers[planet].size(); i++) {
        float layerValue = currentGalaxy.layers[planet][i].value;
        inPlanet ^= pointInCircle(
            currentGalaxy.positions[entity].x,
            currentGalaxy.positions[entity].y,
            currentGalaxy.positions[planet].x,
            currentGalaxy.positions[planet].y,
            layerValue
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
        if( pointInWall(
            currentGalaxy.positions[entity].x,
            currentGalaxy.positions[entity].y,
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
            currentGalaxy.positions[entity].x,
            currentGalaxy.positions[entity].y,
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

void Collider::entitiesPlatforms(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.entities.size(); i++) {
        size_t entityIndex = currentGalaxy.entities[i].index;
        float px = currentGalaxy.positions[entityIndex].x;
        float py = currentGalaxy.positions[entityIndex].y;

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
                return;
            }
        }
        currentGalaxy.platformIndexes[entityIndex] = -1;
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

void Collider::updateCollisions(Galaxy& currentGalaxy) {
    entitiesPlanets(currentGalaxy);
    entitiesPlatforms(currentGalaxy);
}