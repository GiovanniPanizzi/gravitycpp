#pragma once 
#include "../../objects/Galaxy.hpp"

class Collider {
    public:
    Collider();
    ~Collider();

    bool humanInPlanet(Galaxy& currentGalaxy, size_t humanIndex, size_t planet);
    bool humanPlatform(Galaxy& currentGalaxy, size_t humanIndex, size_t platformIndex);
    void updateHumanCollisions(Galaxy& currentGalaxy, size_t humanIndex);
    void updateHumansCollisions(Galaxy& currentGalaxy);
};