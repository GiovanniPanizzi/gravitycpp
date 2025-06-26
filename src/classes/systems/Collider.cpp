#include "../../../include/classes/systems/Collider.hpp"
#include <iostream>

Collider::Collider() {}
Collider::~Collider() {}

bool Collider::humanInPlanet(Galaxy& currentGalaxy, size_t humanIndex, size_t planet){
    Vec2 humanFuturePosition = currentGalaxy.humans.positions[humanIndex];
    Circle planetCircle;
    planetCircle.position = currentGalaxy.planets.positions[planet];
    planetCircle.radius = currentGalaxy.planets.radii[planet];
    Rect humanRect;
    humanRect.position = {humanFuturePosition.x - currentGalaxy.humans.sizes[humanIndex].width / 2, humanFuturePosition.y - currentGalaxy.humans.sizes[humanIndex].height};
    humanRect.size = currentGalaxy.humans.sizes[humanIndex];
    float humanAngle = currentGalaxy.humans.angles[humanIndex].rad;
    Vec2 pivot = {humanFuturePosition.x, humanFuturePosition.y};

    if(!isRectInCircle(humanRect, humanAngle, pivot, planetCircle)) {
        return false;
    }
    if(currentGalaxy.planets.layers.empty()) {

    }
    return false;
}

void Collider::updateHumanCollisions(Galaxy& currentGalaxy, size_t humanIndex){
    for(size_t i = 0; i < currentGalaxy.planets.entities.size(); i++) {
        currentGalaxy.humans.planetIndexes[humanIndex] = -1;
        if(humanInPlanet(currentGalaxy, humanIndex, i)) {
            currentGalaxy.humans.planetIndexes[humanIndex] = i;
        }
    }
}
