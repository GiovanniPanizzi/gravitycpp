#pragma once
#include "Galaxy.h"

class Collider {
    public:
    bool pointInCircle(float pointX, float pointY, float circleX, float circleY, float r);
    bool pointInPlatform(float pointX, float pointY, float circleX, float circleY, float planetR, float height, float width, float angle);
    bool pointInWall(float pointX, float pointY, float circleX, float circleY, float innerR, float outerR, float width, float angleDeg);
    bool pointInPlanet(size_t planet, size_t entity, Galaxy& currentGalaxy);
    void entitiesPlatforms(Galaxy& currentGalaxy);
    void entitiesPlanets(Galaxy& currentGalaxy);
    void updateCollisions(Galaxy& currentGalaxy);
};