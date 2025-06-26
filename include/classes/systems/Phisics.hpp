#pragma once
#include "Collider.hpp"

class Phisics {
    public:

    void update(Galaxy& currentGalaxy);

    void updatePlatformsRotation(Galaxy& currentGalaxy);
    void updateEntitiesRotation(Galaxy& currentGalaxy);
    void updateEntitiesMotion(Galaxy& currentGalaxy);

    void updateEntitiesVelocityOnPlatforms(Galaxy& currentGalaxy);

    void updateEntityGravitalForce(Galaxy& currentGalaxy, size_t entityId);
    void updateEntityAcceleration(Galaxy& currentGalaxy, size_t entityId);
    void updateEntityRotation(Galaxy& currentGalaxy, size_t entityId);
    void updateEntityMotion(Galaxy& currentGalaxy, size_t entityId);
    void updateEntityOverlap(Galaxy& currentGalaxy, size_t entityId);

    void updateWormPositions(Galaxy& currentGalaxy, size_t wormId);
    void updateWorms(Galaxy& currentGalaxy);
    void updateWormGravity(Galaxy& currentGalaxy, size_t wormId);
    void updateWormMotion(Galaxy& currentGalaxy, size_t wormId);
    void updateWormOverlap(Galaxy& currentGalaxy, size_t wormId);

    void entitiesUpdate(Galaxy& currentGalaxy);

};