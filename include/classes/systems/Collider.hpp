#pragma once 
#include "../../objects/Galaxy.hpp"

class Collider {
    public:
    Collider();
    ~Collider();

    bool humanInPlanet(Galaxy& currentGalaxy, size_t humanIndex, size_t planet);
    void updateHumanCollisions(Galaxy& currentGalaxy, size_t humanIndex);
    void updateHumansCollisions(Galaxy& currentGalaxy);
};