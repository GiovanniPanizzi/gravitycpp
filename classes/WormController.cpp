#include "../headers/WormController.h"

int32_t value = 0;

void WormController::moveLeft(Galaxy& currentGalaxy, size_t wormIndex){
    if(currentGalaxy.wormPlanetIndexes[wormIndex][0] != -1){
        float dx = currentGalaxy.wormPositions[wormIndex][0].x - currentGalaxy.positions[currentGalaxy.wormPlanetIndexes[wormIndex][0]].x;
        float dy = currentGalaxy.wormPositions[wormIndex][0].y - currentGalaxy.positions[currentGalaxy.wormPlanetIndexes[wormIndex][0]].y;
        float dist = sqrt(dx * dx + dy * dy);
        currentGalaxy.wormVelocities[wormIndex][0].x += dy * 0.5f / dist;
        currentGalaxy.wormVelocities[wormIndex][0].y -= dx * 0.5f / dist;
    }
}

void WormController::moveRight(Galaxy& currentGalaxy, size_t wormIndex){
    if(currentGalaxy.wormPlanetIndexes[wormIndex][0] != -1){
        float dx = currentGalaxy.wormPositions[wormIndex][0].x - currentGalaxy.positions[currentGalaxy.wormPlanetIndexes[wormIndex][0]].x;
        float dy = currentGalaxy.wormPositions[wormIndex][0].y - currentGalaxy.positions[currentGalaxy.wormPlanetIndexes[wormIndex][0]].y;
        float dist = sqrt(dx * dx + dy * dy);
        currentGalaxy.wormVelocities[wormIndex][0].x -= dy * 0.5f / dist;
        currentGalaxy.wormVelocities[wormIndex][0].y += dx * 0.5f / dist;
    }
}

void WormController::allMove(Galaxy& currentGalaxy){
    for(int i = 0; i < currentGalaxy.worms.size(); i++){
        if(value < 200){
            moveLeft(currentGalaxy, currentGalaxy.worms[i].index);
        }
        else{
            moveRight(currentGalaxy, currentGalaxy.worms[i].index);
        }
        value++;
    }
}