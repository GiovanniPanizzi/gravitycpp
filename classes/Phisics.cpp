#include "../headers/Phisics.h"
#include "../headers/Collider.h"
#include "../headers/EntityController.h"
#include <iostream>

Collider collider;

const int G = 35;

/*methods*/
//kinetics for platforms
void Phisics::updatePlatformsRotation(Galaxy& currentGalaxy) {
    for(size_t i = 0; i < currentGalaxy.platforms.size(); i++){
        size_t index = currentGalaxy.platforms[i].index;
        currentGalaxy.angles[index].deg += currentGalaxy.angularSpeeds[index];
    }
}

//update entity kinetics
void Phisics::updateEntityGravitalForce(Galaxy& currentGalaxy, size_t entityId) {
    if(currentGalaxy.planetIndexes[entityId] != -1) return;
    for(size_t j = 0; j < currentGalaxy.planets.size(); j++){
        size_t planetId = currentGalaxy.planets[j].index;
        float distanceX = currentGalaxy.positions[entityId].x - currentGalaxy.positions[planetId].x;
        float distanceY = currentGalaxy.positions[entityId].y - currentGalaxy.positions[planetId].y;
        float distSquared = distanceX * distanceX + distanceY * distanceY;
        float dist = sqrt(distSquared);
        float force = currentGalaxy.masses[planetId].value * G / distSquared;
        currentGalaxy.accelerations[entityId].x -= force * distanceX / dist;
        currentGalaxy.accelerations[entityId].y -= force * distanceY / dist;
    }
}

void Phisics::updateEntityRotation(Galaxy& currentGalaxy, size_t entityId) {
    int planetId = currentGalaxy.planetIndexes[entityId];
    if(planetId == -1){
        currentGalaxy.angles[entityId].deg = atan2(currentGalaxy.accelerations[entityId].y,
        currentGalaxy.accelerations[entityId].x) * 180.0f / M_PI - 90.0f;
    }
    else{
        float dx = currentGalaxy.positions[planetId].x - currentGalaxy.positions[entityId].x;
        float dy = currentGalaxy.positions[planetId].y - currentGalaxy.positions[entityId].y;
        currentGalaxy.angles[entityId].deg = std::atan2(dy, dx) * 180.0f / M_PI - 90.0f;
    }
}

void Phisics::updateEntityMotion(Galaxy& currentGalaxy, size_t entityId) {
    int platformId = currentGalaxy.platformIndexes[entityId];

    if(platformId != -1){
        float omega = currentGalaxy.angularSpeeds[platformId] * M_PI / 180.0f;
        int planetId = currentGalaxy.planetIndexes[platformId];
        Vec2& planetPos = currentGalaxy.positions[planetId];
        Vec2& entityPos = currentGalaxy.positions[entityId];

        float dx = entityPos.x - planetPos.x;
        float dy = entityPos.y - planetPos.y;
        float r = std::sqrt(dx*dx + dy*dy);

        float theta = std::atan2(dy, dx);

        // velocità tangenziale piattaforma
        float vx = -omega * r * std::sin(theta);
        float vy = omega * r * std::cos(theta);

        currentGalaxy.relativeVelocities[entityId].x = vx;
        currentGalaxy.relativeVelocities[entityId].y = vy;
    }
    else{
        if(currentGalaxy.planetIndexes[entityId] != -1){
            currentGalaxy.relativeVelocities[entityId].x = 0.0f;
            currentGalaxy.relativeVelocities[entityId].y = 0.0f;
        }
    }

    currentGalaxy.velocities[entityId].x += currentGalaxy.accelerations[entityId].x;
    currentGalaxy.velocities[entityId].y += currentGalaxy.accelerations[entityId].y;
}

void Phisics::updateEntityAcceleration(Galaxy& currentGalaxy, size_t entityId) {
    currentGalaxy.accelerations[entityId].x = 0.0f;
    currentGalaxy.accelerations[entityId].y = 0.0f;
    updateEntityGravitalForce(currentGalaxy, entityId);
}

void Phisics::updateEntityOverlap(Galaxy& currentGalaxy, size_t entityId) {
    int planetId = currentGalaxy.planetIndexes[entityId];
    int platformId = currentGalaxy.platformIndexes[entityId];

    Vec2& entityPos = currentGalaxy.positions[entityId];
    Vec2& planetPos = currentGalaxy.positions[planetId];
    float dx = entityPos.x - planetPos.x;
    float dy = entityPos.y - planetPos.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    // Handle entity inside a wall
    if (currentGalaxy.wallIndexes[entityId] != -1) {

        // Stop further movement into the wall
        currentGalaxy.velocities[entityId].x = 0;
        currentGalaxy.velocities[entityId].y = 0;

        size_t wallIndex = currentGalaxy.wallIndexes[entityId];
        int hollowPlanetId = currentGalaxy.planetIndexes[wallIndex];

        // Calculate the distance and angle from the planet center
        float dx = currentGalaxy.positions[entityId].x - currentGalaxy.positions[hollowPlanetId].x + currentGalaxy.velocities[entityId].x;
        float dy = currentGalaxy.positions[entityId].y - currentGalaxy.positions[hollowPlanetId].y + currentGalaxy.velocities[entityId].y;
        float dist = std::sqrt(dx * dx + dy * dy);
        // Calculate the angle in radians
        float angleRad = std::atan2(dy, dx);
        float entityAngleDeg = angleRad * 180.0f / M_PI;
        float wallAngleDeg = currentGalaxy.angles[wallIndex].deg;

        float deltaAngle = entityAngleDeg - wallAngleDeg;

        // Normalizza a [-180, 180]
        while (deltaAngle < -180.0f) deltaAngle += 360.0f;
        while (deltaAngle > 180.0f) deltaAngle -= 360.0f;

        // Se deltaAngle > 0 → entità a destra del muro, spingi in senso orario
        // Se deltaAngle < 0 → entità a sinistra, spingi in senso antiorario
        float PUSH_DISTANCE = (deltaAngle > 0) ? 2.0f : -2.0f;


        // Calculate the tangential direction (perpendicular to the radial direction)
        float tangentX = -dy / dist; // Perpendicular to radial direction
        float tangentY = dx / dist;  // Perpendicular to radial direction

        // Push the entity tangentially
        currentGalaxy.positions[entityId].x += tangentX * PUSH_DISTANCE;
        currentGalaxy.positions[entityId].y += tangentY * PUSH_DISTANCE;

        return;
    }

    // Handle entity inside a planet
    if(planetId != -1){
        Radius radius = currentGalaxy.radii[planetId];
        for(size_t i = 0; i < currentGalaxy.layers[planetId].size(); i++){
            if(dist < currentGalaxy.layers[planetId][i].value + 5){
                radius.value = currentGalaxy.layers[planetId][i].value;
            }
        }
        if(platformId != -1){
            radius.value += currentGalaxy.sizes[platformId].height;
        }
        Vec2 parallelVelocity;
        Vec2 perpendicularVelocity;
        parallelVelocity = velocityTowardsPoint(planetPos, entityPos, currentGalaxy.velocities[entityId]);
        bool direction = (dot(parallelVelocity, subtract(planetPos, entityPos)) > 0); 

        if(dist != radius.value){
            entityPos.x = planetPos.x - std::cos(currentGalaxy.angles[entityId].deg * M_PI / 180.0f + M_PI / 2) * radius.value;
            entityPos.y = planetPos.y - std::sin(currentGalaxy.angles[entityId].deg * M_PI / 180.0f + M_PI / 2) * radius.value;
        }

        currentGalaxy.velocities[entityId].x *= currentGalaxy.frictions[planetId].value;
        currentGalaxy.velocities[entityId].y *= currentGalaxy.frictions[planetId].value;
    }
}

void Phisics::entitiesUpdate(Galaxy& currentGalaxy){
    for(size_t i = 0; i < currentGalaxy.entities.size(); i++){
        updateEntityRotation(currentGalaxy, currentGalaxy.entities[i].index);
        if(currentGalaxy.jumpStaminas[currentGalaxy.entities[i].index].value > 5){
            updateEntityOverlap(currentGalaxy, currentGalaxy.entities[i].index);
        }
        if(currentGalaxy.jumpStaminas[currentGalaxy.entities[i].index].value < currentGalaxy.jumpStaminas[currentGalaxy.entities[i].index].maxValue){
        currentGalaxy.jumpStaminas[currentGalaxy.entities[i].index].value++;
        }
        if(currentGalaxy.moveStamina[currentGalaxy.entities[i].index].value < currentGalaxy.moveStamina[currentGalaxy.entities[i].index].maxValue){
            currentGalaxy.moveStamina[currentGalaxy.entities[i].index].value++;
        }
    }
    collider.updateCollisions(currentGalaxy);
    for(size_t i = 0; i < currentGalaxy.entities.size(); i++){
        size_t entityId = currentGalaxy.entities[i].index;
        float totalVx = currentGalaxy.velocities[entityId].x + currentGalaxy.relativeVelocities[entityId].x;
        float totalVy = currentGalaxy.velocities[entityId].y + currentGalaxy.relativeVelocities[entityId].y;

        currentGalaxy.positions[entityId].x += totalVx;
        currentGalaxy.positions[entityId].y += totalVy;

        updateEntityMotion(currentGalaxy, currentGalaxy.entities[i].index);
        updateEntityAcceleration(currentGalaxy, currentGalaxy.entities[i].index);

        //update hitbox
        float radAngle = currentGalaxy.angles[entityId].deg * M_PI / 180.0f;
        float offSetX = cos(radAngle) * 30 * currentGalaxy.directions[entityId].value;
        float offSetY = sin(radAngle) * 30 * currentGalaxy.directions[entityId].value;
        Vec2 offSet = {offSetX, offSetY};
        currentGalaxy.hitBoxes[entityId].position = sum(currentGalaxy.positions[entityId], offSet);
    }
}

void Phisics::updateWormPositions(Galaxy& currentGalaxy, size_t wormId) {
    std::vector<Vec2>& positions = currentGalaxy.wormPositions[wormId];
    std::vector<Radius>& radii = currentGalaxy.wormRadii[wormId];

    // 1. Aggiorna la testa
    Vec2& head = positions[0];

    // 2. Ogni segmento segue il precedente a distanza del raggio precedente
    for (size_t i = 1; i < positions.size(); ++i) {
        Vec2& current = positions[i];
        const Vec2& previous = positions[i - 1];
        float distance = radii[i - 1].value; // solo il raggio del cerchio precedente

        Vec2 dir = subtract(previous, current);
        float len = length(dir);

        if (len > radii[i - 1].value) {
            Vec2 norm = normalize(dir);
            current = sum(previous, multiply(norm, -distance));
        }

        if(i != 1){
            const Vec2& A = currentGalaxy.wormPositions[wormId][i - 2];
            const Vec2& B = currentGalaxy.wormPositions[wormId][i - 1];
            Vec2& C = currentGalaxy.wormPositions[wormId][i];

            Vec2 v1 = subtract(B, A);
            Vec2 v2 = subtract(C, B);

            float len1 = length(v1);
            float len2 = length(v2);

            if (len1 > 0.0001f && len2 > 0.0001f) {
                float dotProduct = dot(v1, v2);
                float angle = std::acos(dotProduct / (len1 * len2));

                float angleDegrees = angle * (180.0f / M_PI);

                /*if(angleDegrees > 90.0f){
                    C = rotatePointAroundOrigin(C, 20.0f, B);
                }
                if(angleDegrees < -90.0f){
                    C = rotatePointAroundOrigin(C, -20.0f, B);
                }
                else if(angleDegrees > 0){
                    C = rotatePointAroundOrigin(C, -1.0f, B);
                }
                else if(angleDegrees < 0){
                    C = rotatePointAroundOrigin(C, 1.0f, B);
                }*/
            }
        }
    }
}

void Phisics::updateWormMotion(Galaxy& currentGalaxy, size_t wormId) {
    for(int i = 0; i < currentGalaxy.wormPositions[wormId].size(); i++){
        currentGalaxy.wormPositions[wormId][i] = sum(currentGalaxy.wormVelocities[wormId][i], currentGalaxy.wormPositions[wormId][i]);
        Vec2& velocity = currentGalaxy.wormVelocities[wormId][i];
        velocity = sum(velocity, currentGalaxy.wormAccelerations[wormId][i]);
    }
}

void Phisics::updateWormGravity(Galaxy& currentGalaxy, size_t wormId) {
    for(int i = 0; i < currentGalaxy.wormPositions[wormId].size(); i++){
        if(currentGalaxy.wormPlanetIndexes[wormId][i] != -1) continue;
        for(size_t j = 0; j < currentGalaxy.planets.size(); j++){
            Vec2& position = currentGalaxy.wormPositions[wormId][i];
            size_t planetId = currentGalaxy.planets[j].index;
            float distanceX = position.x - currentGalaxy.positions[planetId].x;
            float distanceY = position.y - currentGalaxy.positions[planetId].y;
            float distSquared = distanceX * distanceX + distanceY * distanceY;
            float dist = sqrt(distSquared);
            float force = currentGalaxy.masses[planetId].value * G / distSquared;
            currentGalaxy.wormAccelerations[wormId][i].x -= force * distanceX / dist;
            currentGalaxy.wormAccelerations[wormId][i].y -= force * distanceY / dist;
        }
    }
}

void Phisics::updateWormOverlap(Galaxy& currentGalaxy, size_t wormId) {
    for(int j = 0; j < currentGalaxy.wormPositions[wormId].size(); j++){
        Vec2& position = currentGalaxy.wormPositions[wormId][j];
        int planetId = currentGalaxy.wormPlanetIndexes[wormId][j];
        if (planetId == -1) continue;

        Radius radius = currentGalaxy.radii[planetId];
        for (size_t i = 0; i < currentGalaxy.layers[planetId].size(); i++) {
            if (length(subtract(position, currentGalaxy.positions[planetId])) < currentGalaxy.layers[planetId][i].value + 5) {
                radius.value = currentGalaxy.layers[planetId][i].value;
            }
        }

        Vec2 dir = normalize(subtract(position, currentGalaxy.positions[planetId]));
        position = sum(currentGalaxy.positions[planetId], multiply(dir, radius.value));
        currentGalaxy.wormVelocities[wormId][j] = multiply(currentGalaxy.wormVelocities[wormId][j], currentGalaxy.frictions[planetId].value);
    }
}

void Phisics::updateWorms(Galaxy& currentGalaxy) {
    for(int i = 0; i < currentGalaxy.worms.size(); i++){
        collider.wormsPlanets(currentGalaxy);
        size_t wormId = currentGalaxy.worms[i].index;
        updateWormMotion(currentGalaxy, wormId);
        for(int j = 0; j < currentGalaxy.wormAccelerations[wormId].size(); j++){
            currentGalaxy.wormAccelerations[wormId][j].x = 0.0f;
            currentGalaxy.wormAccelerations[wormId][j].y = 0.0f;
        }
        updateWormOverlap(currentGalaxy, wormId);
        updateWormPositions(currentGalaxy, wormId);
        updateWormGravity(currentGalaxy, wormId);
    }
}

//update
void Phisics::update(Galaxy& currentGalaxy){
    updatePlatformsRotation(currentGalaxy);
    entitiesUpdate(currentGalaxy);
    updateWorms(currentGalaxy);
}