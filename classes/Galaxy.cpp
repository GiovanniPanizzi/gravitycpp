#include "../headers/Galaxy.h"
#include <string>
#include <iostream>

Galaxy::Galaxy(){
    Object player = {0};  
    entities.push_back(player);
    positions[0] = (Position{0.0f, 0.0f});
    velocities[0] = (Velocity{0.0f, 0.0f});
    relativeVelocities[0] = (Velocity{0, 0});
    accelerations[0] = (Acceleration{0.0f, 0.0f});
    sizes[0] = (Size{100, 200});
    angles[0] = (Angle{0.0f});
    planetIndexes[0] = (-1);
    platformIndexes[0] = (-1);
    wallIndexes[0] = (-1);
    jumpStaminas[0] = {50, 50}; 
    moveStamina[0] = {5, 5};
    index = 1;
}

void Galaxy::addPlanet(Position position, Velocity velocity, Acceleration acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<Radius> planetLayers) {
    planets.push_back({index});
    positions[index] = (position);
    velocities[index] = (velocity);
    accelerations[index] = (acceleration);
    radii[index] = (radius);
    masses[index] = (mass);
    frictions[index] = (friction);
    elasticities[index] = (elasticity);
    angles[index] = {0.0f};
    layers[index] = planetLayers;
    planetPlatforms[index] = {};
    layerEntries[index] = {};
    layerWalls[index] = {};

    index++;
}

void Galaxy::addEntity(Position position, Velocity velocity, Acceleration acceleration, Size size, Angle angle) {
    entities.push_back({index});
    positions[index] = (position);
    velocities[index] = (velocity);
    relativeVelocities[index] = (Velocity{0, 0});
    accelerations[index] = (acceleration);
    sizes[index] = (size);
    angles[index] = (angle);
    planetIndexes[index] = (-1);
    platformIndexes[index] = (-1);
    wallIndexes[index] = (-1);
    jumpStaminas[index] = {30, 30};
    moveStamina[index] = {5, 5};
    index++;
}

void Galaxy::addPlanetPlatform(Size size, size_t planetIndex, Angle angle, float angularSpeed){
    platforms.push_back({index});
    sizes[index] = (size);
    angles[index] = (angle);
    planetIndexes[index] = (planetIndex);
    angularSpeeds[index] = (angularSpeed);
    if (planetIndex >= 0) {
        planetPlatforms[planetIndex].push_back(index);
    }
    index++;
}

void Galaxy::addPlanetWall(size_t planetIndex, int planetStartLayer, int planetEndLayer, int width, Angle angle) {
    walls.push_back({index});
    startLayers[index] = planetStartLayer;
    endLayers[index] = planetEndLayer;
    widths[index] = width;
    angles[index] = (angle);
    planetIndexes[index] = (planetIndex);
    if (planetIndex >= 0) {
        layerWalls[planetIndex].push_back(index);
    }
    index++;
}

void Galaxy::addPlanetEntry(size_t planetIndex, int planetStartLayer, int width, Angle angle) {
    entries.push_back({index});
    startLayers[index] = planetStartLayer;
    widths[index] = width;
    angles[index] = (angle);
    planetIndexes[index] = (planetIndex);
    if (planetIndex >= 0) {
        layerEntries[planetIndex].push_back(index);
    }
    index++;
}

void Galaxy::removePlanetPlatform(size_t indexDelete){
    for (auto it = platforms.begin(); it != platforms.end(); ++it) {
        if (it->index == indexDelete) {
            platforms.erase(it);
            break;
        }
    }
    sizes.erase(indexDelete);
    angles.erase(indexDelete);
    planetIndexes.erase(indexDelete);
    angularSpeeds.erase(indexDelete);
}

void Galaxy::removePlanet(size_t indexDelete) {
    for (auto it = planets.begin(); it != planets.end(); ++it) {
        if (it->index == indexDelete) {
            planets.erase(it);
            break;
        }
    }
    positions.erase(indexDelete);
    velocities.erase(indexDelete);
    accelerations.erase(indexDelete);
    radii.erase(indexDelete);
    masses.erase(indexDelete);
    frictions.erase(indexDelete);
    elasticities.erase(indexDelete);
    angles.erase(indexDelete);
    layers.erase(indexDelete);
    layerEntries.erase(indexDelete);
    layerWalls.erase(indexDelete);
    planetPlatforms.erase(indexDelete);
}

void Galaxy::removeEntity(size_t indexDelete) {
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        if (it->index == indexDelete) {
            entities.erase(it);
            break;
        }
    }
    positions.erase(indexDelete);
    velocities.erase(indexDelete);
    relativeVelocities.erase(indexDelete);
    accelerations.erase(indexDelete);
    sizes.erase(indexDelete);
    angles.erase(indexDelete);
    planetIndexes.erase(indexDelete);
    platformIndexes.erase(indexDelete);
    jumpStaminas.erase(indexDelete);
    moveStamina.erase(indexDelete);
}

void rotatePointAroundOrigin(int& x, int& y, float angleDeg, float pivotX, float pivotY) {

    double angleRad = angleDeg * M_PI / 180.0;
    double cosA = cos(angleRad);
    double sinA = sin(angleRad);

    // Trasla il punto rispetto al centro
    double tx = x - pivotX;
    double ty = y - pivotY;

    double rx = tx * cosA - ty * sinA;
    double ry = tx * sinA + ty * cosA;

    // Riporta il punto alla posizione originale
    x = static_cast<int>(rx + pivotX);
    y = static_cast<int>(ry + pivotY);
}

bool isCircleVisible(int drawX, int drawY, float radius) {
    return !(drawX + radius < 0 || drawX - radius > screenWidth ||
             drawY + radius < 0 || drawY - radius > screenHeight);
}

void Galaxy::draw(Draw& draw){

    Position playerPosition = positions[0];

    draw.clearScreen(30, 5, 100, 255); 

    for (size_t i = 0; i < planets.size(); i++) {
        size_t index = planets[i].index;
        Radius planetRadius = radii[index];
        Position planetPosition = positions[index];

        int drawX = (positions[index].x - playerPosition.x) * scale + screenWidth / 2; 
        int drawY = (positions[index].y - playerPosition.y) * scale + screenHeight / 2;

        //rotatePointAroundOrigin(drawX, drawY, angles[0].deg, screenWidth / 2 + 100, screenHeight / 2);
        if(!isCircleVisible(drawX, drawY, planetRadius.value * scale)) continue;
        draw.drawFilledCircle(drawX, drawY, planetRadius.value * scale, 120, 80, 4, 255);
        for(int i = 0; i < layers[index].size(); i++){
            float layerRadius = layers[index][i].value;
            if(!isCircleVisible(drawX, drawY, layerRadius * scale)) continue;
            if(i % 2 == 0){
                draw.drawFilledCircle(drawX, drawY, layerRadius * scale, 0, 0, 200, 255);
            }
            else{
                draw.drawFilledCircle(drawX, drawY, layerRadius * scale, 90, 60, 3, 255);
            }
        }
    }

    for(size_t i = 0; i < walls.size(); i++) {
        size_t index = walls[i].index;
        int planetIndex = planetIndexes[index];
        if (planetIndex < 0) continue;

        float angleDeg = angles[index].deg;
        int startLayer = startLayers[index];
        int endLayer = endLayers[index];
        int width = widths[index];

        float planetX = positions[planetIndex].x;
        float planetY = positions[planetIndex].y;

        float radius = radii[planetIndex].value; 

        float innerRadius = layers[planetIndex][startLayer].value;
        float outerRadius = layers[planetIndex][endLayer].value;

        float startAngle = angleDeg - width / 2.0f;
        float endAngle = angleDeg + width / 2.0f;

        int drawX = (planetX - playerPosition.x) * scale + screenWidth / 2.0f;
        int drawY = (planetY - playerPosition.y) * scale + screenHeight / 2.0f;

        if(!isCircleVisible(drawX, drawY, outerRadius * scale)) continue;

        draw.drawPlatform(drawX, drawY, innerRadius * scale, outerRadius * scale, startAngle, endAngle, 90, 60, 3, 255);
    }

    for (size_t i = 0; i < entries.size(); i++) {
        size_t index = entries[i].index;
        int planetIndex = planetIndexes[index];
        if (planetIndex < 0) continue;

        float angleDeg = angles[index].deg;
        int startLayer = startLayers[index];
        int width = widths[index];

        float planetX = positions[planetIndex].x;
        float planetY = positions[planetIndex].y;

        float radius = radii[planetIndex].value; 

        float innerRadius = layers[planetIndex][startLayer].value;
        float outerRadius;
        if(startLayer == 0){
            outerRadius = radius;
        }else{
            outerRadius = layers[planetIndex][startLayer - 1].value;
        }

        float startAngle = angleDeg - width / 2.0f;
        float endAngle = angleDeg + width / 2.0f;

        int drawX = (planetX - playerPosition.x) * scale + screenWidth / 2.0f;
        int drawY = (planetY - playerPosition.y) * scale + screenHeight / 2.0f;

        if(!isCircleVisible(drawX, drawY, outerRadius * scale)) continue;

        draw.drawPlatform(drawX, drawY, innerRadius * scale, outerRadius * scale, startAngle, endAngle, 0, 0, 200, 255);
    }

    for (size_t i = 0; i < platforms.size(); i++) {
        size_t index = platforms[i].index;

        float angleDeg = angles[index].deg;
        float widthDeg = sizes[index].width;
        float height = sizes[index].height;

        int planetIndex = planetIndexes[index];
        if (planetIndex < 0) continue;

        float planetX = positions[planetIndex].x;
        float planetY = positions[planetIndex].y;

        float radius = radii[planetIndex].value; 

        float innerRadius = radius;
        float outerRadius = radius + height;

        float startAngle = angleDeg - widthDeg / 2.0f;
        float endAngle = angleDeg + widthDeg / 2.0f;

        int drawX = (planetX - playerPosition.x) * scale + screenWidth / 2.0f;
        int drawY = (planetY - playerPosition.y) * scale + screenHeight / 2.0f;

        if(!isCircleVisible(drawX, drawY, outerRadius * scale)) continue;

        //rotatePointAroundOrigin(drawX, drawY, angles[0].deg, screenWidth / 2, screenHeight / 2);

        draw.drawPlatform(drawX, drawY, innerRadius * scale, outerRadius * scale, startAngle, endAngle, 255, 255, 0, 255);
    }

    for (size_t i = 0; i < entities.size(); i++) {
        size_t index = entities[i].index;
        Size entitySize = sizes[index];
        int pivotX = entitySize.width * scale / 2;  
        int pivotY = entitySize.height * scale; 
        int drawX = (positions[index].x - playerPosition.x) * scale - entitySize.width * scale / 2 + screenWidth / 2; 
        int drawY = (positions[index].y - playerPosition.y) * scale -entitySize.height * scale + screenHeight / 2;
        float angleDeg = angles[index].deg;
        draw.drawFilledRotatedRect(drawX, drawY, entitySize.width * scale, entitySize.height * scale, angleDeg, pivotX, pivotY, 255, 0, 0, 255);
    }
}