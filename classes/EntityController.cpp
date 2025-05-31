#include "../headers/EntityController.h"
#include <iostream>

float velocity = 6.0f;
float jumpJump = 10.0f;

void EntityController::moveLeft(Galaxy& currentGalaxy, size_t entityIndex){
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
    if(currentGalaxy.jumpStaminas[entityIndex].value >= currentGalaxy.jumpStaminas[entityIndex].maxValue){
        int planetId = currentGalaxy.planetIndexes[entityIndex];
        if(currentGalaxy.planetIndexes[entityIndex] != -1 || currentGalaxy.platformIndexes[entityIndex] != -1){
            currentGalaxy.jumpStaminas[entityIndex].value = 0;
            float dx = currentGalaxy.positions[entityIndex].x - currentGalaxy.positions[planetId].x;
            float dy = currentGalaxy.positions[entityIndex].y - currentGalaxy.positions[planetId].y;
            float len = sqrt(dx * dx + dy * dy);
            dx /= len;
            dy /= len;
            currentGalaxy.velocities[entityIndex].x += dx * jumpJump;
            currentGalaxy.velocities[entityIndex].y += dy * jumpJump;
        }
    }
}