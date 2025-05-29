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

bool Collider::pointInEntry(float pointX, float pointY,
                               float circleX, float circleY,
                               float outerR, float innerR,
                               float width, float angleDeg) {
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

bool Collider::pointInHollowPlanet(float pointX, float pointY, float circleX, float circleY, float planetRadius, std::vector<Radius>& hollowRadii, std::vector<layerEntry>& layerEntries, std::vector<layerEntry>& walls, bool& isInsideWall) {
    // Check if the point is inside the main planet radius
    bool inside = pointInCircle(pointX, pointY, circleX, circleY, planetRadius);

    // XOR with each hollow radius
    for (const Radius& hollowRadius : hollowRadii) {
        inside ^= pointInCircle(pointX, pointY, circleX, circleY, hollowRadius.value + 5);
    }
    // layerEntries
    for (const layerEntry& entry : layerEntries) {
        float innerR = hollowRadii[entry.depth].value;
        float outerR;
        if(entry.depth == 0){
            outerR = planetRadius + 5;
        }
        else{
            outerR = hollowRadii[entry.depth - 1].value + 5;
        }
        if (pointInEntry(pointX, pointY, circleX, circleY, outerR, innerR, entry.width, entry.angle.deg)) {
            return false;
        }
    }
    // walls
    for (const layerEntry& wall : walls) {
        float innerR = hollowRadii[wall.depth].value;
        float outerR;
        if(wall.depth == 0){
            outerR = planetRadius;
        }
        else{
            outerR = hollowRadii[wall.depth - 1].value;
        }
        if (pointInEntry(pointX, pointY, circleX, circleY, outerR, innerR - 5, wall.width + 5, wall.angle.deg)) {
            isInsideWall = true;
            return true;
        }
    }
    return inside;
}

void Collider::entitiesPlatforms(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.entities.size(); i++) {
        size_t entityIndex = currentGalaxy.entities[i].i;
        float px = currentGalaxy.positions[entityIndex].x;
        float py = currentGalaxy.positions[entityIndex].y;

        for (size_t j = 0; j < currentGalaxy.platforms.size(); j++) {
            size_t platformIndex = currentGalaxy.platforms[j].i;

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
        size_t entityIndex = currentGalaxy.entities[i].i;
        float px = currentGalaxy.positions[entityIndex].x;
        float py = currentGalaxy.positions[entityIndex].y;

        int currentPlanetId = currentGalaxy.planetIndexes[entityIndex];

        currentGalaxy.planetIndexes[entityIndex] = -1;

        for (size_t j = 0; j < currentGalaxy.planets.size(); j++) {
            size_t planetIndex = currentGalaxy.planets[j].i;
            float cx = currentGalaxy.positions[planetIndex].x;
            float cy = currentGalaxy.positions[planetIndex].y;
            float r = currentGalaxy.radii[planetIndex].value;

            if (pointInCircle(px, py, cx, cy, r + 5)) {
                currentGalaxy.planetIndexes[entityIndex] = planetIndex;
                return;
            }
        }

        currentGalaxy.hollowPlanetIndexes[entityIndex] = -1;
        currentGalaxy.isInsideWall[entityIndex] = false;

        for (size_t j = 0; j < currentGalaxy.hollowPlanets.size(); j++) {
            size_t planetIndex = currentGalaxy.hollowPlanets[j].i;
            float cx = currentGalaxy.positions[planetIndex].x;
            float cy = currentGalaxy.positions[planetIndex].y;
            float r = currentGalaxy.radii[planetIndex].value;

            if (pointInHollowPlanet(px, py, cx, cy, r + 5, currentGalaxy.hollowPlanetsRadii[planetIndex], currentGalaxy.planetsLayerEntries[planetIndex], currentGalaxy.holloPlanetswalls[planetIndex], currentGalaxy.isInsideWall[entityIndex])) {
                currentGalaxy.hollowPlanetIndexes[entityIndex] = planetIndex;
                return;
            }
        }
    }
}

void Collider::updateCollisions(Galaxy& currentGalaxy) {
    entitiesPlanets(currentGalaxy);
    entitiesPlatforms(currentGalaxy);
}