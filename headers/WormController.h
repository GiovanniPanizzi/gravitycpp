#pragma once
#include "Galaxy.h"

class WormController {
    private:
    public:
    void moveLeft(Galaxy& currentGalaxy, size_t wormIndex);
    void moveRight(Galaxy& currentGalaxy, size_t wormIndex);
    void allMove(Galaxy& currentGalaxy);
};