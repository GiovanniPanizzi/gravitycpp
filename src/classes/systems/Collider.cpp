#include "../../../include/classes/systems/Collider.hpp"
#include <iostream>

Collider::Collider() {}
Collider::~Collider() {}

bool Collider::humanInPlanet(Galaxy& currentGalaxy, size_t humanIndex, size_t planet){
    Vec2& humanPosition = currentGalaxy.humans.positions[humanIndex];
    Vec2 humanFuturePosition = currentGalaxy.humans.positions[humanIndex];
    Circle planetCircle;
    planetCircle.position = currentGalaxy.planets.positions[planet];
    planetCircle.radius.value = currentGalaxy.planets.radii[planet].value;
    Rect humanRect;
    humanRect.position = {humanFuturePosition.x - currentGalaxy.humans.sizes[humanIndex].width / 2, humanFuturePosition.y - currentGalaxy.humans.sizes[humanIndex].height};
    humanRect.size = currentGalaxy.humans.sizes[humanIndex];
    float humanAngle = currentGalaxy.humans.angles[humanIndex].rad;
    Vec2 pivot = {humanFuturePosition.x, humanFuturePosition.y};
    //not in planet
    if(!isRectInCircle(humanRect, humanAngle, pivot, planetCircle)) {
        return false;
    }
    //planet with no layers
    if(currentGalaxy.planets.layers[planet].empty()) {
        Vec2 perpendicularVelocity = velocityTowardsPoint(planetCircle.position, humanPosition, currentGalaxy.humans.velocities[humanIndex]);
        if(length(perpendicularVelocity) > 1) {
            currentGalaxy.humans.velocities[humanIndex] = subtract(currentGalaxy.humans.velocities[humanIndex], perpendicularVelocity);
        }
        currentGalaxy.humans.velocities[humanIndex] = multiply(currentGalaxy.humans.velocities[humanIndex], currentGalaxy.planets.frictions[planet].value);
        Vec2 dir = subtract(humanPosition, planetCircle.position);
        float dist = length(dir);

        Vec2 dirNormalized;
        if (dist == 0) {
            // Se human è al centro del pianeta, spostalo verso destra (asse x positivo)
            dirNormalized = {1.0f, 0.0f};
        } else {
            dirNormalized = normalize(dir);
        }

        humanPosition = add(planetCircle.position, multiply(dirNormalized, planetCircle.radius.value));

        return true;
    }
    //planet with layers
    for(int i = 0; i < currentGalaxy.planets.layers[planet].size(); i++){
        /*std::cout << "HumanRect pos: " << humanRect.position.x << "," << humanRect.position.y << std::endl;
        std::cout << "Size: " << humanRect.size.width << "x" << humanRect.size.height << std::endl;
        std::cout << "Angle: " << humanAngle << std::endl;
        std::cout << "PlanetLayer " << i << " shape: " 
                  << currentGalaxy.planets.layers[planet][i].shape.innerRadius.value << " - "
                  << currentGalaxy.planets.layers[planet][i].shape.outerRadius.value << std::endl;
        std::cout << "Start angle: " << currentGalaxy.planets.layers[planet][i].shape.startAngle.rad << std::endl;
        std::cout << "End angle: " << currentGalaxy.planets.layers[planet][i].shape.endAngle.rad << std::endl;
        std::cout << "Pivot:" << pivot.x << "," << pivot.y << std::endl;
        std::cout << "PlanetCircle pos: " << planetCircle.position.x << "," << planetCircle.position.y << std::endl;*/
        if(isRectInAnnularSection(humanRect, humanAngle, currentGalaxy.planets.layers[planet][i].shape, humanFuturePosition, planetCircle.position)){
            Vec2 perpendicularVelocity = velocityTowardsAnnularSection(humanPosition, currentGalaxy.humans.velocities[humanIndex], currentGalaxy.planets.layers[planet][i].shape, planetCircle.position);
            if(length(perpendicularVelocity) != 0) {
                currentGalaxy.humans.velocities[humanIndex] = subtract(currentGalaxy.humans.velocities[humanIndex], perpendicularVelocity);
            }
            humanPosition = projectOntoAnnularSectionSurface(humanPosition, currentGalaxy.planets.layers[planet][i].shape, planetCircle.position);
            return true;
        }
    }

    return false;
}

void Collider::updateHumanCollisions(Galaxy& currentGalaxy, size_t humanIndex){
    for(size_t i = 0; i < currentGalaxy.planets.entities.size(); i++) {
        currentGalaxy.humans.planetIndexes[humanIndex] = -1;
        if(humanInPlanet(currentGalaxy, humanIndex, i)) {
            currentGalaxy.humans.planetIndexes[humanIndex] = i;
            return;
        }
    }
}

void Collider::updateHumansCollisions(Galaxy& currentGalaxy) {
    for(size_t i = 0; i < currentGalaxy.humans.entities.size(); i++) {
        updateHumanCollisions(currentGalaxy, i);
    }
}
