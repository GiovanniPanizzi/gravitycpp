#pragma once
#include "Galaxy.h"
#include "Collider.h"

class Phisics {
    public:

    void update(Galaxy& currentGalaxy);

    void updatePlatformsRotation(Galaxy& currentGalaxy);
    void updateEntitiesRotation(Galaxy& currentGalaxy);
    void updateEntitiesMotion(Galaxy& currentGalaxy);

    void updateEntitiesVelocityOnPlatforms(Galaxy& currentGalaxy);

    void updateEntityGravitalForce(Galaxy currentGalaxy, size_t entityId);
    void updateEntityRotation(Galaxy& currentGalaxy, size_t entityId);
    void updateEntityMotion(Galaxy& currentGalaxy, size_t entityId);
};