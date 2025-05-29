#include "../headers/Galaxy.h"
#include <string>
#include <iostream>

std::unordered_map<std::string, Elasticity> elasticity;
std::unordered_map<std::string, Friction> friction;
std::unordered_map<std::string, float> density;

Galaxy::Galaxy(){
    Entity player = {0};  
    entities.push_back(player);
    positions[0] = (Position{0.0f, 0.0f});
    velocities[0] = (Velocity{0.0f, 0.0f});
    relativeVelocities[0] = (Velocity{0, 0});
    accelerations[0] = (Acceleration{0.0f, 0.0f});
    sizes[0] = (Size{100, 200});
    angles[0] = (Angle{0.0f});
    planetIndexes[0] = (-1);
    platformIndexes[0] = (-1);
    hollowPlanetIndexes[0] = (-1);
    jumpStaminas[0] = {30, 30}; 
    moveStamina[0] = {5, 5};
    animatedSpriteMap[0] = {nullptr, 0, false};
    isInsideWall[0] = false;
    index = 1;
}

void Galaxy::addPlanet(Position position, Velocity velocity, Acceleration acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity) {
    planets.push_back({index});
    positions[index] = (position);
    velocities[index] = (velocity);
    accelerations[index] = (acceleration);
    radii[index] = (radius);
    masses[index] = (mass);
    frictions[index] = (friction);
    elasticities[index] = (elasticity);
    angles[index] = {0.0f};
    index++;
}

void Galaxy::addHollowPlanet(Position position, Velocity velocity, Acceleration acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<Radius> hollowRadii, std::vector<layerEntry> layerEntries, std::vector<layerEntry> walls) {
    hollowPlanets.push_back({index});
    positions[index] = (position);
    velocities[index] = (velocity);
    accelerations[index] = (acceleration);
    radii[index] = (radius);
    masses[index] = (mass);
    frictions[index] = (friction);
    elasticities[index] = (elasticity);
    angles[index] = {0.0f};
    hollowPlanetsRadii[index] = hollowRadii;
    planetsLayerEntries[index] = layerEntries;
    holloPlanetswalls[index] = walls;
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
    hollowPlanetIndexes[index] = (-1);
    jumpStaminas[index] = {30, 30};
    moveStamina[index] = {5, 5};
    animatedSpriteMap[index] = {nullptr, 0, false};
    isInsideWall[index] = false;
    index++;
}

void Galaxy::addPlanetPlatform(Size size, size_t planetIndex, Angle angle, float angularSpeed){
    platforms.push_back({index});
    sizes[index] = (size);
    angles[index] = (angle);
    planetIndexes[index] = (planetIndex);
    angularSpeeds[index] = (angularSpeed);
    index++;
}

void Galaxy::removePlanetPlatform(size_t indexDelete){
    for (auto it = platforms.begin(); it != platforms.end(); ++it) {
        if (it->i == indexDelete) {
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
        if (it->i == indexDelete) {
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
}

void Galaxy::removeEntity(size_t indexDelete) {
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        if (it->i == indexDelete) {
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
    animatedSpriteMap.erase(indexDelete);
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

void Galaxy::draw(Draw& draw){

    Position playerPosition = positions[0];

    draw.clearScreen(0, 0, 255, 0);

    for (size_t i = 0; i < planets.size(); i++) {
        size_t index = planets[i].i;
        Radius planetRadius = radii[index];
        Position planetPosition = positions[index];

        int drawX = positions[index].x - playerPosition.x + screenWidth / 2; 
        int drawY = positions[index].y - playerPosition.y + screenHeight / 2;

        //rotatePointAroundOrigin(drawX, drawY, angles[0].deg, screenWidth / 2 + 100, screenHeight / 2);
        draw.drawFilledCircle(drawX, drawY, planetRadius.value, 150, 100, 5, 255);
    }

    for (size_t i = 0; i < platforms.size(); i++) {
        size_t index = platforms[i].i;

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

        int drawX = planetX - playerPosition.x + screenWidth / 2.0f;
        int drawY = planetY - playerPosition.y + screenHeight / 2.0f;

        //rotatePointAroundOrigin(drawX, drawY, angles[0].deg, screenWidth / 2, screenHeight / 2);

        draw.drawPlatform(drawX, drawY, innerRadius, outerRadius, startAngle, endAngle, 255, 255, 0, 255);
    }

    for (size_t i = 0; i < hollowPlanets.size(); i++) {
        size_t index = hollowPlanets[i].i;
        Radius planetRadius = radii[index];
        Position planetPosition = positions[index];
        std::vector<Radius>& hollowRadiiForPlanet = hollowPlanetsRadii[index];
        std::vector<layerEntry>& layerEntriesForPlanet = planetsLayerEntries[index];
        int drawX = positions[index].x - playerPosition.x + screenWidth / 2; 
        int drawY = positions[index].y - playerPosition.y + screenHeight / 2;
        draw.drawFilledCircle(drawX, drawY, planetRadius.value, 150, 100, 5, 255);

        for (size_t j = 0; j < hollowRadiiForPlanet.size(); j++) {
            float radius = hollowRadiiForPlanet[j].value;
            if(j % 2 == 0){
                draw.drawFilledCircle(drawX, drawY, radius, 0, 0, 255, 255);
            } else {
                draw.drawFilledCircle(drawX, drawY, radius, 150, 100, 5, 255);
            }
        }

        for (size_t j = 0; j < layerEntriesForPlanet.size(); j++) {
            layerEntry& layer = layerEntriesForPlanet[j];
            Radius radius2;
            if(layer.depth > 0){
                radius2 = hollowRadiiForPlanet[layer.depth - 1];
            }
            else{
                radius2 = planetRadius;
            }

            float startAngle = layer.angle.deg - layer.width / 2.0f;
            float endAngle = layer.angle.deg + layer.width / 2.0f;

            draw.drawPlatform(drawX, drawY, hollowRadiiForPlanet[layer.depth].value, radius2.value, startAngle, endAngle, 0, 0, 255, 255);
        }
        for (size_t j = 0; j < holloPlanetswalls[index].size(); j++) {
            layerEntry& wall = holloPlanetswalls[index][j];
            Radius radius2;
            if(wall.depth > 0){
                radius2 = hollowRadiiForPlanet[wall.depth - 1];
            }
            else{
                radius2 = planetRadius;
            }

            float startAngle = wall.angle.deg - wall.width / 2.0f;
            float endAngle = wall.angle.deg + wall.width / 2.0f;

            draw.drawPlatform(drawX, drawY, hollowRadiiForPlanet[wall.depth].value, radius2.value, startAngle, endAngle, 150, 100, 5, 255);
        }
    }

    for (size_t i = 0; i < entities.size(); i++) {
        size_t index = entities[i].i;
        Size entitySize = sizes[index];
        int pivotX = entitySize.width / 2;  
        int pivotY = entitySize.height; 
        int drawX = positions[index].x - entitySize.width / 2 - playerPosition.x + screenWidth / 2; 
        int drawY = positions[index].y - entitySize.height - playerPosition.y + screenHeight / 2;
        float angleDeg = angles[index].deg; 
        //draw.drawRotatedSprite(*animatedSpriteMap[index].sprite);
        draw.drawFilledRotatedRect(drawX, drawY, entitySize.width, entitySize.height, angleDeg, pivotX, pivotY, 255, 0, 0, 255);
    }
}