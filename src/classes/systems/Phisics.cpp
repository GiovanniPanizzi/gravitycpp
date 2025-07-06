#include "../../../include/classes/systems/Phisics.hpp"
#include <iostream>

Collider collider;

const int G = 35;

/*methods*/
//kinetics for platforms
void Phisics::updatePlatformsRotation(Galaxy& currentGalaxy) {
    for(size_t i = 0; i < currentGalaxy.platforms.entities.size(); i++){
        size_t index = currentGalaxy.platforms.entities[i].index;
        currentGalaxy.platforms.shapes[index].startAngle.rad += currentGalaxy.platforms.angularSpeeds[index];
        currentGalaxy.platforms.shapes[index].endAngle.rad += currentGalaxy.platforms.angularSpeeds[index];
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
        currentGalaxy.humans.angles[entityId].rad = atan2(
            currentGalaxy.humans.accelerations[entityId].y,
            currentGalaxy.humans.accelerations[entityId].x
        ) - M_PI / 2;
    } 
}

void Phisics::updateEntityMotion(Galaxy& currentGalaxy, size_t entityId) {
    int platformId = currentGalaxy.humans.platformIndexes[entityId];

    if (platformId != -1) {
        float omega = currentGalaxy.platforms.angularSpeeds[platformId];
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
    if(currentGalaxy.humans.planetIndexes[entityId] == -1)
    updateEntityGravitalForce(currentGalaxy, entityId);
}

void Phisics::entitiesUpdate(Galaxy& currentGalaxy) { 
    collider.updateHumansCollisions(currentGalaxy);
    // Update rotation, overlap, and stamina for each entity
    for (size_t i = 0; i < currentGalaxy.humans.entities.size(); i++) {

        size_t entityId = i;

        // Regenerate jump stamina
        if (currentGalaxy.humans.jumpStaminas[entityId].value < currentGalaxy.humans.jumpStaminas[entityId].maxValue) {
            currentGalaxy.humans.jumpStaminas[entityId].value++;
        }

        // Regenerate move stamina
        if (currentGalaxy.humans.moveStaminas[entityId].value < currentGalaxy.humans.moveStaminas[entityId].maxValue) {
            currentGalaxy.humans.moveStaminas[entityId].value++;
        }

        // Calculate total velocity
        if(length(currentGalaxy.humans.velocities[entityId]) < 0.01f) {
            currentGalaxy.humans.velocities[entityId].x = 0.0f;
            currentGalaxy.humans.velocities[entityId].y = 0.0f;
        }
        float totalVx = currentGalaxy.humans.velocities[entityId].x + currentGalaxy.humans.relativeVelocities[entityId].x;
        float totalVy = currentGalaxy.humans.velocities[entityId].y + currentGalaxy.humans.relativeVelocities[entityId].y;

        // Update position
        currentGalaxy.humans.positions[entityId].x += totalVx;
        currentGalaxy.humans.positions[entityId].y += totalVy;

        // Update motion and acceleration
        updateEntityMotion(currentGalaxy, entityId);
        updateEntityAcceleration(currentGalaxy, entityId);

        // Update entity rotation
        updateEntityRotation(currentGalaxy, entityId);

        // Update hitbox
        float radAngle = currentGalaxy.humans.angles[entityId].rad;
        float offsetX = cos(radAngle) * 30 * currentGalaxy.humans.directions[entityId].value;
        float offsetY = sin(radAngle) * 30 * currentGalaxy.humans.directions[entityId].value;
        Vec2 offset = {offsetX, offsetY};
        currentGalaxy.humans.hitBoxes[entityId].position = add(currentGalaxy.humans.positions[entityId], offset);
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
            current = add(previous, multiply(norm, -distance));
        }
    }
}

void Phisics::updateWormMotion(Galaxy& currentGalaxy, size_t wormId) {
    for (size_t i = 0; i < currentGalaxy.worms.positions[wormId].size(); i++) {
        // Update position based on velocity
        currentGalaxy.worms.positions[wormId][i] = add(currentGalaxy.worms.velocities[wormId][i], currentGalaxy.worms.positions[wormId][i]);

        // Update velocity based on acceleration
        Vec2& velocity = currentGalaxy.worms.velocities[wormId][i];
        velocity = add(velocity, currentGalaxy.worms.accelerations[wormId][i]);
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

void Phisics::updateWorms(Galaxy& currentGalaxy) {
    for (size_t i = 0; i < currentGalaxy.worms.entities.size(); i++) {
        size_t wormId = currentGalaxy.worms.entities[i].index;

        // Update collisions

        // Update motion
        updateWormMotion(currentGalaxy, wormId);

        // Reset accelerations
        for (size_t j = 0; j < currentGalaxy.worms.accelerations[wormId].size(); j++) {
            currentGalaxy.worms.accelerations[wormId][j].x = 0.0f;
            currentGalaxy.worms.accelerations[wormId][j].y = 0.0f;
        }

        updateWormPositions(currentGalaxy, wormId);
        updateWormGravity(currentGalaxy, wormId);
    }
}

//update
void Phisics::update(Galaxy& currentGalaxy){
    updatePlatformsRotation(currentGalaxy);
    entitiesUpdate(currentGalaxy);
}