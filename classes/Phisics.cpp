#include "../headers/Phisics.h"
#include "../headers/Collider.h"
#include "../headers/EntityController.h"
#include <iostream>

Collider collider;

const int G = 6;

/*functions*/

Velocity makeVelocity(const Position& from, const Position& to) {
    return {to.x - from.x, to.y - from.y};
}

float dot(const Velocity& a, const Velocity& b) {
    return a.x * b.x + a.y * b.y;
}

float normSquared(const Velocity& v) {
    return v.x * v.x + v.y * v.y;
}

Velocity scaleVector(const Velocity& v, float scalar) {
    return {v.x * scalar, v.y * scalar};
}

Velocity subtract(const Velocity& a, const Velocity& b) {
    return {a.x - b.x, a.y - b.y};
}

void decomposeVelocity(const Velocity& v, const Position& p1, const Position& p2,Velocity& vParallela, Velocity& vPerpendicolare, bool& vaVersoDestinazione) {
    Velocity dir = makeVelocity(p1, p2); 
    double alpha = dot(v, dir) / normSquared(dir);
    vParallela = scaleVector(dir, alpha);
    vPerpendicolare = subtract(v, vParallela);

    double verso = dot(vParallela, dir);
    vaVersoDestinazione = verso > 0;
}
/*methods*/
//kinetics for platforms
void Phisics::updatePlatformsRotation(Galaxy& currentGalaxy){
    for(size_t i = 0; i < currentGalaxy.platforms.size(); i++){
        size_t index = currentGalaxy.platforms[i].i;
        currentGalaxy.angles[index].deg += currentGalaxy.angularSpeeds[index];
    }
}

//update entity kinetics
void updateEntityGravitalForce(Galaxy& currentGalaxy, size_t entityId){
    if(currentGalaxy.planetIndexes[entityId] != -1 || currentGalaxy.hollowPlanetIndexes[entityId] != -1) return;
    for(size_t j = 0; j < currentGalaxy.planets.size(); j++){
        size_t planetId = currentGalaxy.planets[j].i;
        float distanceX = currentGalaxy.positions[entityId].x - currentGalaxy.positions[planetId].x;
        float distanceY = currentGalaxy.positions[entityId].y - currentGalaxy.positions[planetId].y;
        float distSquared = distanceX * distanceX + distanceY * distanceY;
        float dist = sqrt(distSquared);
        float force = currentGalaxy.masses[planetId].value * G / distSquared;
        currentGalaxy.accelerations[entityId].x -= force * distanceX / dist;
        currentGalaxy.accelerations[entityId].y -= force * distanceY / dist;
    }
    for(size_t j = 0; j < currentGalaxy.hollowPlanets.size(); j++){
        size_t planetId = currentGalaxy.hollowPlanets[j].i;
        float distanceX = currentGalaxy.positions[entityId].x - currentGalaxy.positions[planetId].x;
        float distanceY = currentGalaxy.positions[entityId].y - currentGalaxy.positions[planetId].y;
        float distSquared = distanceX * distanceX + distanceY * distanceY;
        float dist = sqrt(distSquared);
        float force = currentGalaxy.masses[planetId].value * G / distSquared;
        currentGalaxy.accelerations[entityId].x -= force * distanceX / dist;
        currentGalaxy.accelerations[entityId].y -= force * distanceY / dist;
    }
}

void updateEntityRotation(Galaxy& currentGalaxy, size_t entityId){
    int planetId = currentGalaxy.planetIndexes[entityId];
    if(planetId == -1){
        planetId = currentGalaxy.hollowPlanetIndexes[entityId];
    }
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

void updateEntityMotion(Galaxy& currentGalaxy, size_t entityId) {
    int platformId = currentGalaxy.platformIndexes[entityId];

    if(platformId != -1){
        float omega = currentGalaxy.angularSpeeds[platformId] * M_PI / 180.0f;
        int planetId = currentGalaxy.planetIndexes[platformId];
        Position& planetPos = currentGalaxy.positions[planetId];
        Position& entityPos = currentGalaxy.positions[entityId];

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
        if(currentGalaxy.planetIndexes[entityId] != -1 || currentGalaxy.hollowPlanetIndexes[entityId] != -1){
            currentGalaxy.relativeVelocities[entityId].x = 0.0f;
            currentGalaxy.relativeVelocities[entityId].y = 0.0f;
        }
    }

    currentGalaxy.velocities[entityId].x += currentGalaxy.accelerations[entityId].x;
    currentGalaxy.velocities[entityId].y += currentGalaxy.accelerations[entityId].y;

    float totalVx = currentGalaxy.velocities[entityId].x + currentGalaxy.relativeVelocities[entityId].x;
    float totalVy = currentGalaxy.velocities[entityId].y + currentGalaxy.relativeVelocities[entityId].y;

    currentGalaxy.positions[entityId].x += totalVx;
    currentGalaxy.positions[entityId].y += totalVy;
}

void updateEntityAcceleration(Galaxy& currentGalaxy, size_t entityId){
    currentGalaxy.accelerations[entityId].x = 0.0f;
    currentGalaxy.accelerations[entityId].y = 0.0f;
    updateEntityGravitalForce(currentGalaxy, entityId);
}

void updateEntityOverlap(Galaxy& currentGalaxy, size_t entityId){
    int planetId = currentGalaxy.planetIndexes[entityId];
    int platformId = currentGalaxy.platformIndexes[entityId];
    int hollowPlanetId = currentGalaxy.hollowPlanetIndexes[entityId];
    if(planetId != -1){
        Radius radius = currentGalaxy.radii[planetId];
        if(platformId != -1){
            radius.value += currentGalaxy.sizes[platformId].height;
        }
        Velocity parallelVelocity;
        Velocity perpendicularVelocity;
        bool direction;
        decomposeVelocity(currentGalaxy.velocities[entityId], currentGalaxy.positions[entityId], currentGalaxy.positions[planetId], parallelVelocity, perpendicularVelocity, direction);
        Position& entityPos = currentGalaxy.positions[entityId];
        Position& planetPos = currentGalaxy.positions[planetId];

        float dx = entityPos.x - planetPos.x;
        float dy = entityPos.y - planetPos.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if(dist != radius.value){
            entityPos.x = planetPos.x - std::cos(currentGalaxy.angles[entityId].deg * M_PI / 180.0f + M_PI / 2) * radius.value;
            entityPos.y = planetPos.y - std::sin(currentGalaxy.angles[entityId].deg * M_PI / 180.0f + M_PI / 2) * radius.value;
        }

        currentGalaxy.velocities[entityId].x *= currentGalaxy.frictions[planetId].value;
        currentGalaxy.velocities[entityId].y *= currentGalaxy.frictions[planetId].value;
    }
    if(!currentGalaxy.isInsideWall[entityId]){
        if(hollowPlanetId != -1){
            Radius radius = currentGalaxy.radii[hollowPlanetId];
            Position& entityPos = currentGalaxy.positions[entityId];
            Position& planetPos = currentGalaxy.positions[hollowPlanetId];
            float dx = entityPos.x - planetPos.x;
            float dy = entityPos.y - planetPos.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            for(int i = 0; i < currentGalaxy.hollowPlanetsRadii[hollowPlanetId].size(); i++){
                if(currentGalaxy.hollowPlanetsRadii[hollowPlanetId][i].value <= radius.value && dist < currentGalaxy.hollowPlanetsRadii[hollowPlanetId][i].value + 5 && i % 2 == 1){
                    radius.value = currentGalaxy.hollowPlanetsRadii[hollowPlanetId][i].value;
                }
            }
            if(platformId != -1){
                radius.value += currentGalaxy.sizes[platformId].height;
            }
            Velocity parallelVelocity;
            Velocity perpendicularVelocity;
            bool direction;
            decomposeVelocity(currentGalaxy.velocities[entityId], currentGalaxy.positions[entityId], currentGalaxy.positions[hollowPlanetId], parallelVelocity, perpendicularVelocity, direction);

            if(dist != radius.value){
                entityPos.x = planetPos.x - std::cos(currentGalaxy.angles[entityId].deg * M_PI / 180.0f + M_PI / 2) * radius.value;
                entityPos.y = planetPos.y - std::sin(currentGalaxy.angles[entityId].deg * M_PI / 180.0f + M_PI / 2) * radius.value;
            }

            currentGalaxy.velocities[entityId].x *= currentGalaxy.frictions[hollowPlanetId].value;
            currentGalaxy.velocities[entityId].y *= currentGalaxy.frictions[hollowPlanetId].value;
        }
    }
    // Check if the entity is inside a wall of a hollow planet
    for (size_t i = 0; i < currentGalaxy.holloPlanetswalls[hollowPlanetId].size(); i++) {
        const layerEntry& wall = currentGalaxy.holloPlanetswalls[hollowPlanetId][i];

        // Calculate inner and outer radii for the wall
        float innerR = currentGalaxy.hollowPlanetsRadii[hollowPlanetId][wall.depth].value;
        float outerR = currentGalaxy.hollowPlanetsRadii[hollowPlanetId][wall.depth - 1].value;

        // Check if the entity is inside the wall
        if (collider.pointInEntry(currentGalaxy.positions[entityId].x, currentGalaxy.positions[entityId].y,
                                currentGalaxy.positions[hollowPlanetId].x, currentGalaxy.positions[hollowPlanetId].y,
                                outerR, innerR, wall.width, wall.angle.deg)) {
            currentGalaxy.isInsideWall[entityId] = true;

            // Calculate the distance and angle from the planet center
            float dx = currentGalaxy.positions[entityId].x - currentGalaxy.positions[hollowPlanetId].x;
            float dy = currentGalaxy.positions[entityId].y - currentGalaxy.positions[hollowPlanetId].y;
            float dist = std::sqrt(dx * dx + dy * dy);
            float angleRad = std::atan2(dy, dx);

            // Calculate the tangential direction (perpendicular to the radial direction)
            float tangentX = -dy / dist; // Perpendicular to radial direction
            float tangentY = dx / dist;  // Perpendicular to radial direction

            // Push the entity tangentially
            const float PUSH_DISTANCE = 5.0f; // Adjust this value as needed
            currentGalaxy.positions[entityId].x += tangentX * PUSH_DISTANCE;
            currentGalaxy.positions[entityId].y += tangentY * PUSH_DISTANCE;

            // Stop further movement into the wall
            currentGalaxy.velocities[entityId].x = - tangentX * currentGalaxy.velocities[entityId].x;
            currentGalaxy.velocities[entityId].y = - tangentX * currentGalaxy.velocities[entityId].y;

            break; // Exit the loop once we handle the collision
        } else {
            currentGalaxy.isInsideWall[entityId] = false;
        }
    }
}

void entitiesUpdate(Galaxy& currentGalaxy){
    for(size_t i = 0; i < currentGalaxy.entities.size(); i++){
        collider.updateCollisions(currentGalaxy);
        updateEntityAcceleration(currentGalaxy, currentGalaxy.entities[i].i);
        updateEntityMotion(currentGalaxy, currentGalaxy.entities[i].i);
        updateEntityRotation(currentGalaxy, currentGalaxy.entities[i].i);
        if(currentGalaxy.jumpStaminas[currentGalaxy.entities[i].i].value > 5){
            updateEntityOverlap(currentGalaxy, currentGalaxy.entities[i].i);
        }
        if(currentGalaxy.jumpStaminas[currentGalaxy.entities[i].i].value < currentGalaxy.jumpStaminas[currentGalaxy.entities[i].i].maxValue){
        currentGalaxy.jumpStaminas[currentGalaxy.entities[i].i].value++;
        }
        if(currentGalaxy.moveStamina[currentGalaxy.entities[i].i].value < currentGalaxy.moveStamina[currentGalaxy.entities[i].i].maxValue){
            currentGalaxy.moveStamina[currentGalaxy.entities[i].i].value++;
        }
    }
}

//update
void Phisics::update(Galaxy& currentGalaxy){
    updatePlatformsRotation(currentGalaxy);
    entitiesUpdate(currentGalaxy);
}