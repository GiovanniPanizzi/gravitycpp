#include "../headers/EntityController.h"
#include <iostream>

float velocity = 1.0f;
float jumpJump = 2.0f;

void EntityController::moveLeft(Galaxy& currentGalaxy, size_t entityIndex) {
    currentGalaxy.humans.directions[entityIndex].value = -1;
    if (currentGalaxy.humans.moveStaminas[entityIndex].value >= currentGalaxy.humans.moveStaminas[entityIndex].maxValue) {
        if (currentGalaxy.humans.planetIndexes[entityIndex] != -1 || currentGalaxy.humans.platformIndexes[entityIndex] != -1) {
            currentGalaxy.humans.moveStaminas[entityIndex].value = 0;
            float radAngle = currentGalaxy.humans.angles[entityIndex].deg * M_PI / 180.0f;
            currentGalaxy.humans.velocities[entityIndex].x -= cos(radAngle) * velocity;
            currentGalaxy.humans.velocities[entityIndex].y -= sin(radAngle) * velocity;
        }
    }
}

void EntityController::moveRight(Galaxy& currentGalaxy, size_t entityIndex) {
    currentGalaxy.humans.directions[entityIndex].value = 1;
    if (currentGalaxy.humans.moveStaminas[entityIndex].value >= currentGalaxy.humans.moveStaminas[entityIndex].maxValue) {
        if (currentGalaxy.humans.planetIndexes[entityIndex] != -1 || currentGalaxy.humans.platformIndexes[entityIndex] != -1) {
            currentGalaxy.humans.moveStaminas[entityIndex].value = 0;
            float radAngle = currentGalaxy.humans.angles[entityIndex].deg * M_PI / 180.0f;
            currentGalaxy.humans.velocities[entityIndex].x += cos(radAngle) * velocity;
            currentGalaxy.humans.velocities[entityIndex].y += sin(radAngle) * velocity;
        }
    }
}

void EntityController::jump(Galaxy& currentGalaxy, size_t entityIndex) {
    if (currentGalaxy.humans.jumpStaminas[entityIndex].value >= currentGalaxy.humans.jumpStaminas[entityIndex].maxValue) {
        bool isOnGround = currentGalaxy.humans.planetIndexes[entityIndex] != -1 || currentGalaxy.humans.platformIndexes[entityIndex] != -1;

        if (isOnGround || coyoteTimer > 0) {
            currentGalaxy.humans.jumpStaminas[entityIndex].value = 0;

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


void EntityController::updateControls(Galaxy& currentGalaxy, size_t entityIndex, bool leftPressed, bool rightPressed, bool jumpPressed) {
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
            if (jumpingState > 12.0f) {
                jumpingState = 12.0f;
            }
            jump(currentGalaxy, entityIndex);
        }
    }
}


void EntityController::drawState(Galaxy& currentGalaxy, Draw& draw, size_t entityIndex) {
    draw.drawRect(100, 60, currentGalaxy.humans.jumpStaminas[entityIndex].value, 5, 0, 255, 0, 255);
    draw.drawRect(100 + currentGalaxy.humans.jumpStaminas[entityIndex].value, 60, currentGalaxy.humans.jumpStaminas[entityIndex].maxValue - currentGalaxy.humans.jumpStaminas[entityIndex].value, 5, 255, 0, 0, 255);
    draw.drawRect(100, 80, 20 * jumpingState, 5, 255, 255, 255, 255);
    draw.drawRect(100 + 20 * jumpingState, 80, 240 - 20 * jumpingState, 5, 100, 100, 100, 255);
}