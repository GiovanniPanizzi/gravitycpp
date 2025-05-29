#pragma once
#include "Galaxy.h"

class Collider {
    public:
    bool pointInCircle(float pointX, float pointY, float circleX, float circleY, float r);
    bool pointInPlatform(float pointX, float pointY, float circleX, float circleY, float planetR, float height, float width, float angle);
    bool pointInHollowPlanet(float pointX, float pointY, float circleX, float circleY, float planetRadius, std::vector<Radius>& hollowRadii, std::vector<layerEntry>& layerEntries, std::vector<layerEntry>& walls, bool& isInsideWall);
    bool pointInEntry(float pointX, float pointY, float circleX, float circleY, float outerR, float innerR, float width, float angleDeg);
    void entitiesPlatforms(Galaxy& currentGalaxy);
    void entitiesPlanets(Galaxy& currentGalaxy);
    void updateCollisions(Galaxy& currentGalaxy);
};