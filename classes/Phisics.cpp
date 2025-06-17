#include "../headers/Phisics.h"
#include "../headers/Collider.h"
#include "../headers/EntityController.h"
#include <iostream>

Collider collider;

const int G = 35;

/*methods*/
//kinetics for platforms
void Phisics::updatePlatformsRotation(Galaxy& currentGalaxy) {
    for(size_t i = 0; i < currentGalaxy.platforms.entities.size(); i++){
        size_t index = currentGalaxy.platforms.entities[i].index;
        currentGalaxy.platforms.angles[index].deg += currentGalaxy.platforms.angularSpeeds[index];
    }
}

//update entity kinetics
void Phisics::updateEntityGravitalForce(Galaxy& currentGalaxy, size_t entityId) {
    if(currentGalaxy.humans.planetIndexes[entityId] != -1) {
        return;
    }
    Vec2& entityPos = currentGalaxy.humans.positions[entityId];
    Vec2& entityAccel = currentGalaxy.humans.accelerations[entityId];

    // Reset acceleration
    entityAccel.x = 0.0f;
    entityAccel.y = 0.0f;

    for (size_t i = 0; i < currentGalaxy.planets.entities.size(); i++) {
        Vec2& planetPos = currentGalaxy.planets.positions[i];
        float planetMass = currentGalaxy.planets.masses[i].value;

        // Calculate the distance between the entity and the planet
        float dx = planetPos.x - entityPos.x;
        float dy = planetPos.y - entityPos.y;
        float distSquared = dx * dx + dy * dy;

        // Avoid division by zero
        if (distSquared == 0.0f) continue;

        float dist = std::sqrt(distSquared);

        // Calculate gravitational force
        float force = G * planetMass / distSquared;

        // Add the gravitational acceleration to the entity
        entityAccel.x += force * dx / dist;
        entityAccel.y += force * dy / dist;
    }
}

void Phisics::updateEntityRotation(Galaxy& currentGalaxy, size_t entityId) {
    int planetId = currentGalaxy.humans.planetIndexes[entityId];
    if (planetId == -1) {
        currentGalaxy.humans.angles[entityId].deg = atan2(
            currentGalaxy.humans.accelerations[entityId].y,
            currentGalaxy.humans.accelerations[entityId].x
        ) * 180.0f / M_PI - 90.0f;
    } else {
        float dx = currentGalaxy.planets.positions[planetId].x - currentGalaxy.humans.positions[entityId].x;
        float dy = currentGalaxy.planets.positions[planetId].y - currentGalaxy.humans.positions[entityId].y;
        currentGalaxy.humans.angles[entityId].deg = std::atan2(dy, dx) * 180.0f / M_PI - 90.0f;
    }
}

void Phisics::updateEntityMotion(Galaxy& currentGalaxy, size_t entityId) {
    int platformId = currentGalaxy.humans.platformIndexes[entityId];

    if (platformId != -1) {
        float omega = currentGalaxy.platforms.angularSpeeds[platformId] * M_PI / 180.0f;
        int planetId = currentGalaxy.platforms.planetIndexes[platformId];
        Vec2& planetPos = currentGalaxy.planets.positions[planetId];
        Vec2& entityPos = currentGalaxy.humans.positions[entityId];

        float dx = entityPos.x - planetPos.x;
        float dy = entityPos.y - planetPos.y;
        float r = std::sqrt(dx * dx + dy * dy);

        float theta = std::atan2(dy, dx);

        // Tangential velocity of the platform
        float vx = -omega * r * std::sin(theta);
        float vy = omega * r * std::cos(theta);

        currentGalaxy.humans.relativeVelocities[entityId].x = vx;
        currentGalaxy.humans.relativeVelocities[entityId].y = vy;
    } else {
        if (currentGalaxy.humans.planetIndexes[entityId] != -1) {
            currentGalaxy.humans.relativeVelocities[entityId].x = 0.0f;
            currentGalaxy.humans.relativeVelocities[entityId].y = 0.0f;
        }
    }

    currentGalaxy.humans.velocities[entityId].x += currentGalaxy.humans.accelerations[entityId].x;
    currentGalaxy.humans.velocities[entityId].y += currentGalaxy.humans.accelerations[entityId].y;
}

void Phisics::updateEntityAcceleration(Galaxy& currentGalaxy, size_t entityId) {
    currentGalaxy.humans.accelerations[entityId].x = 0.0f;
    currentGalaxy.humans.accelerations[entityId].y = 0.0f;
    updateEntityGravitalForce(currentGalaxy, entityId);
}

void Phisics::updateEntityOverlap(Galaxy& currentGalaxy, size_t entityId) {
    int planetId = currentGalaxy.humans.planetIndexes[entityId];
    int platformId = currentGalaxy.humans.platformIndexes[entityId];

    Vec2& entityPos = currentGalaxy.humans.positions[entityId];
    Vec2 planetPos;
    if (planetId != -1) {
        planetPos = currentGalaxy.planets.positions[planetId];
    } else {
        planetPos = {0.0f, 0.0f};
    }

    float dx = entityPos.x - planetPos.x;
    float dy = entityPos.y - planetPos.y;
    float dist = std::sqrt(dx * dx + dy * dy);

    // Handle entity inside a wall
    if (currentGalaxy.humans.wallIndexes[entityId] != -1) {
        currentGalaxy.humans.velocities[entityId].x = 0;
        currentGalaxy.humans.velocities[entityId].y = 0;

        size_t wallIndex = currentGalaxy.humans.wallIndexes[entityId];
        int hollowPlanetId = currentGalaxy.walls.planetIndexes[wallIndex];

        float dx = currentGalaxy.humans.positions[entityId].x - currentGalaxy.planets.positions[hollowPlanetId].x;
        float dy = currentGalaxy.humans.positions[entityId].y - currentGalaxy.planets.positions[hollowPlanetId].y;
        float dist = std::sqrt(dx * dx + dy * dy);

        float angleRad = std::atan2(dy, dx);
        float entityAngleDeg = angleRad * 180.0f / M_PI;
        float wallAngleDeg = currentGalaxy.planets.angles[wallIndex].deg;

        float deltaAngle = entityAngleDeg - wallAngleDeg;

        while (deltaAngle < -180.0f) deltaAngle += 360.0f;
        while (deltaAngle > 180.0f) deltaAngle -= 360.0f;

        float PUSH_DISTANCE = (deltaAngle > 0) ? 2.0f : -2.0f;

        float tangentX = -dy / dist;
        float tangentY = dx / dist;

        currentGalaxy.humans.positions[entityId].x += tangentX * PUSH_DISTANCE;
        currentGalaxy.humans.positions[entityId].y += tangentY * PUSH_DISTANCE;

        return;
    }

    // Handle entity inside a planet
    if (planetId != -1) {
        Radius radius = currentGalaxy.planets.radii[planetId];
        for (size_t i = 0; i < currentGalaxy.planets.layers[planetId].size(); i++) {
            if (dist < currentGalaxy.planets.layers[planetId][i].value + 5) {
                radius.value = currentGalaxy.planets.layers[planetId][i].value;
            }
        }
        if (platformId != -1) {
            radius.value += currentGalaxy.platforms.sizes[platformId].height;
        }
        Vec2 parallelVelocity;
        Vec2 perpendicularVelocity;
        parallelVelocity = velocityTowardsPoint(planetPos, entityPos, currentGalaxy.humans.velocities[entityId]);
        bool direction = (dot(parallelVelocity, subtract(planetPos, entityPos)) > 0);

        if (dist != radius.value) {
            entityPos.x = planetPos.x - std::cos(currentGalaxy.humans.angles[entityId].deg * M_PI / 180.0f + M_PI / 2) * radius.value;
            entityPos.y = planetPos.y - std::sin(currentGalaxy.humans.angles[entityId].deg * M_PI / 180.0f + M_PI / 2) * radius.value;
        }

        currentGalaxy.humans.velocities[entityId].x *= currentGalaxy.planets.frictions[planetId].value;
        currentGalaxy.humans.velocities[entityId].y *= currentGalaxy.planets.frictions[planetId].value;
    }
}

void Phisics::entitiesUpdate(Galaxy& currentGalaxy) {
    // Update rotation, overlap, and stamina for each entity
    for (size_t i = 0; i < currentGalaxy.humans.entities.size(); i++) {
        size_t entityId = i;

        // Update entity rotation
        updateEntityRotation(currentGalaxy, entityId);

        // Handle overlap if jump stamina is sufficient
        if (currentGalaxy.humans.jumpStaminas[entityId].value > 5) {
            updateEntityOverlap(currentGalaxy, entityId);
        }

        // Regenerate jump stamina
        if (currentGalaxy.humans.jumpStaminas[entityId].value < currentGalaxy.humans.jumpStaminas[entityId].maxValue) {
            currentGalaxy.humans.jumpStaminas[entityId].value++;
        }

        // Regenerate move stamina
        if (currentGalaxy.humans.moveStaminas[entityId].value < currentGalaxy.humans.moveStaminas[entityId].maxValue) {
            currentGalaxy.humans.moveStaminas[entityId].value++;
        }
    }

    // Update collisions
    collider.updateCollisions(currentGalaxy);

    // Update motion, acceleration, and hitboxes for each entity
    for (size_t i = 0; i < currentGalaxy.humans.entities.size(); i++) {
        size_t entityId = currentGalaxy.humans.entities[i].index;

        // Calculate total velocity
        float totalVx = currentGalaxy.humans.velocities[entityId].x + currentGalaxy.humans.relativeVelocities[entityId].x;
        float totalVy = currentGalaxy.humans.velocities[entityId].y + currentGalaxy.humans.relativeVelocities[entityId].y;

        // Update position
        currentGalaxy.humans.positions[entityId].x += totalVx;
        currentGalaxy.humans.positions[entityId].y += totalVy;

        // Update motion and acceleration
        updateEntityMotion(currentGalaxy, entityId);
        updateEntityAcceleration(currentGalaxy, entityId);

        // Update hitbox
        float radAngle = currentGalaxy.humans.angles[entityId].deg * M_PI / 180.0f;
        float offsetX = cos(radAngle) * 30 * currentGalaxy.humans.directions[entityId].value;
        float offsetY = sin(radAngle) * 30 * currentGalaxy.humans.directions[entityId].value;
        Vec2 offset = {offsetX, offsetY};
        currentGalaxy.humans.hitBoxes[entityId].position = sum(currentGalaxy.humans.positions[entityId], offset);
    }
}

void Phisics::updateWormPositions(Galaxy& currentGalaxy, size_t wormId) {
    std::vector<Vec2>& positions = currentGalaxy.worms.positions[wormId];
    std::vector<Radius>& radii = currentGalaxy.worms.radii[wormId];

    // Each segment follows the previous one at a distance equal to the previous segment's radius
    for (size_t i = 1; i < positions.size(); ++i) {
        Vec2& current = positions[i];
        const Vec2& previous = positions[i - 1];
        float distance = radii[i - 1].value;

        Vec2 dir = subtract(previous, current);
        float len = length(dir);

        if (len > distance) {
            Vec2 norm = normalize(dir);
            current = sum(previous, multiply(norm, -distance));
        }
    }
}

void Phisics::updateWormMotion(Galaxy& currentGalaxy, size_t wormId) {
    for (size_t i = 0; i < currentGalaxy.worms.positions[wormId].size(); i++) {
        // Update position based on velocity
        currentGalaxy.worms.positions[wormId][i] = sum(currentGalaxy.worms.velocities[wormId][i], currentGalaxy.worms.positions[wormId][i]);

        // Update velocity based on acceleration
        Vec2& velocity = currentGalaxy.worms.velocities[wormId][i];
        velocity = sum(velocity, currentGalaxy.worms.accelerations[wormId][i]);
    }
}

void Phisics::updateWormGravity(Galaxy& currentGalaxy, size_t wormId) {
    for (size_t i = 0; i < currentGalaxy.worms.positions[wormId].size(); i++) {
        if (currentGalaxy.worms.planetIndexes[wormId][i] != -1) continue;

        for (size_t j = 0; j < currentGalaxy.planets.entities.size(); j++) {
            Vec2& position = currentGalaxy.worms.positions[wormId][i];
            Vec2& planetPos = currentGalaxy.planets.positions[j];
            float planetMass = currentGalaxy.planets.masses[j].value;

            // Calculate gravitational force
            float dx = position.x - planetPos.x;
            float dy = position.y - planetPos.y;
            float distSquared = dx * dx + dy * dy;

            if (distSquared == 0.0f) continue;

            float dist = std::sqrt(distSquared);
            float force = G * planetMass / distSquared;

            // Apply gravitational acceleration
            currentGalaxy.worms.accelerations[wormId][i].x -= force * dx / dist;
            currentGalaxy.worms.accelerations[wormId][i].y -= force * dy / dist;
        }
    }
}

void Phisics::updateWormOverlap(Galaxy& currentGalaxy, size_t wormId) {
    for (size_t j = 0; j < currentGalaxy.worms.positions[wormId].size(); j++) {
        Vec2& position = currentGalaxy.worms.positions[wormId][j];
        int planetId = currentGalaxy.worms.planetIndexes[wormId][j];
        if (planetId == -1) continue;

        Radius radius = currentGalaxy.planets.radii[planetId];
        for (size_t i = 0; i < currentGalaxy.planets.layers[planetId].size(); i++) {
            if (length(subtract(position, currentGalaxy.planets.positions[planetId])) < currentGalaxy.planets.layers[planetId][i].value + 5) {
                radius.value = currentGalaxy.planets.layers[planetId][i].value;
            }
        }

        Vec2 dir = normalize(subtract(position, currentGalaxy.planets.positions[planetId]));
        position = sum(currentGalaxy.planets.positions[planetId], multiply(dir, radius.value));
        currentGalaxy.worms.velocities[wormId][j] = multiply(currentGalaxy.worms.velocities[wormId][j], currentGalaxy.planets.frictions[planetId].value);
    }
}

void Phisics::updateWorms(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.worms.entities.size(); i++) {
        size_t wormId = currentGalaxy.worms.entities[i].index;

        // Update collisions
        collider.wormsPlanets(currentGalaxy);

        // Update motion
        updateWormMotion(currentGalaxy, wormId);

        // Reset accelerations
        for (size_t j = 0; j < currentGalaxy.worms.accelerations[wormId].size(); j++) {
            currentGalaxy.worms.accelerations[wormId][j].x = 0.0f;
            currentGalaxy.worms.accelerations[wormId][j].y = 0.0f;
        }

        // Update overlap, positions, and gravity
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