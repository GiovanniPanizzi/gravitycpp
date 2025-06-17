#include "../headers/EntityController.h"
#include <iostream>

float velocity = 1.0f;
float jumpJump = 2.0f;

void EntityController::moveLeft(Galaxy& currentGalaxy, size_t entityIndex){
    currentGalaxy.directions[entityIndex].value = -1;
    if(currentGalaxy.moveStamina[entityIndex].value >= currentGalaxy.moveStamina[entityIndex].maxValue){
        if(currentGalaxy.planetIndexes[entityIndex] != -1 || currentGalaxy.platformIndexes[entityIndex] != -1){
            currentGalaxy.moveStamina[entityIndex].value = 0;
            float radAngle = currentGalaxy.angles[entityIndex].deg * M_PI / 180.0f;
            currentGalaxy.velocities[entityIndex].x -= cos(radAngle) * velocity;
            currentGalaxy.velocities[entityIndex].y -= sin(radAngle) * velocity;
        }
    }
}

void EntityController::moveRight(Galaxy& currentGalaxy, size_t entityIndex){
    currentGalaxy.directions[entityIndex].value = 1; 
    if(currentGalaxy.moveStamina[entityIndex].value >= currentGalaxy.moveStamina[entityIndex].maxValue){
        if(currentGalaxy.planetIndexes[entityIndex] != -1 || currentGalaxy.platformIndexes[entityIndex] != -1){
            currentGalaxy.moveStamina[entityIndex].value = 0;
            float radAngle = currentGalaxy.angles[entityIndex].deg * M_PI / 180.0f;
            currentGalaxy.velocities[entityIndex].x += cos(radAngle) * velocity;
            currentGalaxy.velocities[entityIndex].y += sin(radAngle) * velocity;
        }
    }
}

void EntityController::jump(Galaxy& currentGalaxy, size_t entityIndex){
    if (currentGalaxy.jumpStaminas[entityIndex].value >= currentGalaxy.jumpStaminas[entityIndex].maxValue) {
        bool isOnGround = currentGalaxy.planetIndexes[entityIndex] != -1 || currentGalaxy.platformIndexes[entityIndex] != -1;

        if (isOnGround || coyoteTimer > 0) {
            currentGalaxy.jumpStaminas[entityIndex].value = 0;

            float dx;
            float dy;
            if(isOnGround){
                dx = currentGalaxy.positions[entityIndex].x - currentGalaxy.positions[currentGalaxy.planetIndexes[entityIndex]].x;
                dy = currentGalaxy.positions[entityIndex].y - currentGalaxy.positions[currentGalaxy.planetIndexes[entityIndex]].y;
            }
            else{
                dx = dxMemory;
                dy = dyMemory;
            }
            float len = sqrt(dx * dx + dy * dy);
            dx /= len;
            dy /= len;

            currentGalaxy.velocities[entityIndex].x += dx * jumpingState;
            currentGalaxy.velocities[entityIndex].y += dy * jumpingState;
            jumpTimer = 0;
            coyoteTimer = 0;
        }
    }
    jumpingState = 0.0f;
}


void EntityController::updateControls(Galaxy& currentGalaxy, size_t entityIndex, bool leftPressed, bool rightPressed, bool jumpPressed) {
    if (currentGalaxy.planetIndexes[entityIndex] != -1 || currentGalaxy.platformIndexes[entityIndex] != -1) {
        coyoteTimer = coyoteTimeMax;
        dxMemory = currentGalaxy.positions[entityIndex].x - currentGalaxy.positions[currentGalaxy.planetIndexes[entityIndex]].x;
        dyMemory = currentGalaxy.positions[entityIndex].y - currentGalaxy.positions[currentGalaxy.planetIndexes[entityIndex]].y;
    } else {
        if (coyoteTimer > 0) coyoteTimer--;
    }

    if (leftPressed) {
        EntityController::moveLeft(currentGalaxy, entityIndex);
    } else if (rightPressed) {
        EntityController::moveRight(currentGalaxy, entityIndex);
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
    draw.drawRect(100, 60, currentGalaxy.jumpStaminas[entityIndex].value, 5, 0, 255, 0, 255);
    draw.drawRect(100 + currentGalaxy.jumpStaminas[entityIndex].value, 60, currentGalaxy.jumpStaminas[entityIndex].maxValue - currentGalaxy.jumpStaminas[entityIndex].value, 5, 255, 0, 0, 255);
    draw.drawRect(100, 80, 20 * jumpingState, 5, 255, 255, 255, 255);
    draw.drawRect(100 + 20 * jumpingState, 80, 240 - 20 * jumpingState, 5, 100, 100, 100, 255);
}