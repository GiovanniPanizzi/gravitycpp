#pragma once
#include "Galaxy.h"

class EntityController {
    public:
    void moveLeft(Galaxy& currentGalaxy, size_t entityIndex);
    void moveRight(Galaxy& currentGalaxy, size_t entityIndex);
    void jump(Galaxy& currentGalaxy, size_t entityIndex);
};

