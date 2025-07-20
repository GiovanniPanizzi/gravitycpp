#include "../../../include/classes/systems/HumanController.hpp"
#include <iostream>

float velocity = 0.6f;
float jumpJump = 2.0f;

void HumanController::moveLeft(Galaxy& currentGalaxy, size_t entityIndex) {
    currentGalaxy.humans.directions[entityIndex].value = -1;
    if (currentGalaxy.humans.moveStaminas[entityIndex].value >= currentGalaxy.humans.moveStaminas[entityIndex].maxValue) {
        if (currentGalaxy.humans.planetIndexes[entityIndex] != -1 || currentGalaxy.humans.platformIndexes[entityIndex] != -1) {
            currentGalaxy.humans.moveStaminas[entityIndex].value = 0;
            float radAngle = currentGalaxy.humans.angles[entityIndex].rad
            ;
            currentGalaxy.humans.velocities[entityIndex].x -= cos(radAngle) * velocity;
            currentGalaxy.humans.velocities[entityIndex].y -= sin(radAngle) * velocity;
        }
    }
}

void HumanController::moveRight(Galaxy& currentGalaxy, size_t entityIndex) {
    currentGalaxy.humans.directions[entityIndex].value = 1;
    if (currentGalaxy.humans.moveStaminas[entityIndex].value >= currentGalaxy.humans.moveStaminas[entityIndex].maxValue) {
        if (currentGalaxy.humans.planetIndexes[entityIndex] != -1 || currentGalaxy.humans.platformIndexes[entityIndex] != -1) {
            currentGalaxy.humans.moveStaminas[entityIndex].value = 0;
            float radAngle = currentGalaxy.humans.angles[entityIndex].rad;
            currentGalaxy.humans.velocities[entityIndex].x += cos(radAngle) * velocity;
            currentGalaxy.humans.velocities[entityIndex].y += sin(radAngle) * velocity;
        }
    }
}

void HumanController::jump(Galaxy& currentGalaxy, size_t entityIndex) {
    if (currentGalaxy.humans.jumpStaminas[entityIndex].value >= currentGalaxy.humans.jumpStaminas[entityIndex].maxValue) {
        bool isOnGround = currentGalaxy.humans.planetIndexes[entityIndex] != -1 || currentGalaxy.humans.platformIndexes[entityIndex] != -1;

        if (isOnGround || coyoteTimer > 0) {
            currentGalaxy.humans.jumpStaminas[entityIndex].value = 0.0f;

            float dx;
            float dy;
            if (isOnGround) {
                dx = currentGalaxy.humans.positions[entityIndex].x - currentGalaxy.planets.positions[currentGalaxy.humans.planetIndexes[entityIndex]].x;
                dy = currentGalaxy.humans.positions[entityIndex].y - currentGalaxy.planets.positions[currentGalaxy.humans.planetIndexes[entityIndex]].y;
            } else {
                dx = dxMemory;
                dy = dyMemory;
            }
            float len = sqrt(dx * dx + dy * dy);
            dx /= len;
            dy /= len;

            currentGalaxy.humans.velocities[entityIndex].x += dx * jumpingState;
            currentGalaxy.humans.velocities[entityIndex].y += dy * jumpingState;
            jumpTimer = 0;
            coyoteTimer = 0;
        }
    }
    jumpingState = 0.0f;
}


void HumanController::updateControls(Galaxy& currentGalaxy, size_t entityIndex, bool leftPressed, bool rightPressed, bool jumpPressed) {
    if (currentGalaxy.humans.planetIndexes[entityIndex] != -1 || currentGalaxy.humans.platformIndexes[entityIndex] != -1) {
        coyoteTimer = coyoteTimeMax;
        dxMemory = currentGalaxy.humans.positions[entityIndex].x - currentGalaxy.planets.positions[currentGalaxy.humans.planetIndexes[entityIndex]].x;
        dyMemory = currentGalaxy.humans.positions[entityIndex].y - currentGalaxy.planets.positions[currentGalaxy.humans.planetIndexes[entityIndex]].y;
    } else {
        if (coyoteTimer > 0) coyoteTimer--;
    }

    if (leftPressed) {
        moveLeft(currentGalaxy, entityIndex);
    } else if (rightPressed) {
        moveRight(currentGalaxy, entityIndex);
    }

    if (jumpPressed) {
        if (jumpingState < 12.0f) {
            jumpingState += (12.0f - jumpingState) / 25.0f;
        }
    } else {
        if (jumpingState > 0.0f) {
            if(jumpingState < 5.0f){
                jumpingState = 5.0f;
            }
            if (jumpingState > 12.0f) {
                jumpingState = 12.0f;
            }
            jump(currentGalaxy, entityIndex);
        }
    }
}