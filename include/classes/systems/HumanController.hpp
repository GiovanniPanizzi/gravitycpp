#pragma once
#include "../generalPurpose/Draw.hpp"
#include "../../objects/Galaxy.hpp"

class HumanController {
    private:
    float jumpingState = 0.0f;
    int jumpTimer = 0;
    float cachedJumpingState = 0.0f;
    int coyoteTimer = 0;
    const int coyoteTimeMax = 6;
    float dxMemory = 0.0f;
    float dyMemory = 0.0f;
    public:
    void moveLeft(Galaxy& currentGalaxy, size_t entityIndex);
    void moveRight(Galaxy& currentGalaxy, size_t entityIndex);
    void jump(Galaxy& currentGalaxy, size_t entityIndex);
    void updateControls(Galaxy& currentGalaxy, size_t entityIndex, bool leftPressed, bool rightPressed, bool jumpPressed);
    void drawState(Galaxy& currentGalaxy, Draw& draw, size_t entityIndex);
};

