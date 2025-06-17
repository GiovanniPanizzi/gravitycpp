#include "../headers/Galaxy.h"
#include <string>
#include <iostream>

Galaxy::Galaxy(){
    Object player = {0};  
    entities.push_back(player);
    positions[0] = (Vec2{0.0f, 0.0f});
    velocities[0] = (Vec2{0.0f, 0.0f});
    relativeVelocities[0] = (Vec2{0, 0});
    accelerations[0] = (Vec2{0.0f, 0.0f});
    sizes[0] = (Size{20, 45});
    angles[0] = (Angle{0.0f});
    planetIndexes[0] = (-1);
    platformIndexes[0] = (-1);
    wallIndexes[0] = (-1);
    jumpStaminas[0] = {50, 50}; 
    moveStamina[0] = {1, 1};
    directions[0] = {1};
    index = 1;
    // Initialize the animated sprite for the player
    animatedSpriteMap[0] = {nullptr, 0, false};
    hitBoxes[0] = {{0.0f, 0.0f}, 40.0f};
    lifePoints[0] = {10, 10};
}

void Galaxy::addPlanet(Vec2 position, Vec2 velocity, Vec2 acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<Radius> planetLayers) {
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

void Galaxy::addEntity(Vec2 position, Vec2 velocity, Vec2 acceleration, Size size, Angle angle) {
    entities.push_back({index});
    positions[index] = (position);
    velocities[index] = (velocity);
    relativeVelocities[index] = (Vec2{0, 0});
    accelerations[index] = (acceleration);
    sizes[index] = (size);
    angles[index] = (angle);
    planetIndexes[index] = (-1);
    platformIndexes[index] = (-1);
    wallIndexes[index] = (-1);
    jumpStaminas[index] = {30, 30};
    moveStamina[index] = {5, 5};
    attackStamina[index] = {20, 20};
    animatedSpriteMap[index] = {nullptr, 0, false};
    hitBoxes[index] = {{0.0f, 0.0f}, 40.0f};
    lifePoints[index] = {10, 10};
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

void Galaxy::addWorm(Vec2 position, std::vector<Radius> wormWormRadii) {
    worms.push_back({index});
    wormPositions[index].push_back(position);
    for(int i = 1; i < wormWormRadii.size(); i++){
        wormPositions[index].push_back(Vec2{wormPositions[index][i - 1].x + wormWormRadii[i - 1].value, position.y});
    }
    wormRadii[index] = wormWormRadii;
    for(int i = 0; i < wormWormRadii.size(); i++){
        wormVelocities[index].push_back(Vec2{0.0f, 0.0f});
    }
    relativeVelocities[index] = (Vec2{0, 0});
    for(int i = 0; i < wormWormRadii.size(); i++){
        wormAccelerations[index].push_back(Vec2{0.0f, 0.0f});
    }
    for(int i = 0; i < wormWormRadii.size(); i++){
        wormPlanetIndexes[index].push_back(-1);
        wormAngles[index].push_back(0.0f);
    }
    planetIndexes[index] = (-1);
    wallIndexes[index] = (-1);
    platformIndexes[index] = (-1);
    hitBoxes[index] = {{0.0f, 0.0f}, 30.0f};
    lifePoints[index] = {5, 5};
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

    if (planetPlatforms.count(indexDelete)) {
        for (size_t platformId : planetPlatforms[indexDelete]) {
            removePlanetPlatform(platformId);
        }
        planetPlatforms.erase(indexDelete);
    }

    std::vector<size_t> entityKeys;
    for (const std::pair<const int, int>& pair : planetIndexes) {
        if (pair.second == static_cast<int>(indexDelete)) {
            entityKeys.push_back(pair.first);
        }
    }
    for (size_t entityId : entityKeys) {
        planetIndexes.erase(entityId);
        platformIndexes.erase(entityId);
        wallIndexes.erase(entityId);
    }

    if (layerWalls.count(indexDelete)) {
        for (size_t wallId : layerWalls[indexDelete]) {
            // Rimuovi wall da vector walls
            walls.erase(std::remove_if(walls.begin(), walls.end(),
                        [wallId](const Object& w) { return w.index == wallId; }),
                        walls.end());

            startLayers.erase(wallId);
            endLayers.erase(wallId);
            widths.erase(wallId);
            angles.erase(wallId);
            planetIndexes.erase(wallId);
        }
        layerWalls.erase(indexDelete);
    }

    if (layerEntries.count(indexDelete)) {
        for (size_t entryId : layerEntries[indexDelete]) {
            entries.erase(std::remove_if(entries.begin(), entries.end(),
                          [entryId](const Object& e) { return e.index == entryId; }),
                          entries.end());

            startLayers.erase(entryId);
            widths.erase(entryId);
            angles.erase(entryId);
            planetIndexes.erase(entryId);
        }
        layerEntries.erase(indexDelete);
    }

    // 5. Rimuovi dalla lista dei pianeti
    planets.erase(std::remove_if(planets.begin(), planets.end(),
                  [indexDelete](const Object& p) { return p.index == indexDelete; }),
                  planets.end());

    // 6. Cancella tutte le proprietà legate al pianeta
    positions.erase(indexDelete);
    velocities.erase(indexDelete);
    relativeVelocities.erase(indexDelete);
    accelerations.erase(indexDelete);
    gravities.erase(indexDelete);
    sizes.erase(indexDelete);
    radii.erase(indexDelete);
    angles.erase(indexDelete);
    masses.erase(indexDelete);
    frictions.erase(indexDelete);
    elasticities.erase(indexDelete);
    layers.erase(indexDelete);
    startLayers.erase(indexDelete);
    endLayers.erase(indexDelete);
    widths.erase(indexDelete);
    heights.erase(indexDelete);
    directions.erase(indexDelete);
    jumpStaminas.erase(indexDelete);
    moveStamina.erase(indexDelete);
    animatedSpriteMap.erase(indexDelete);
    planetIndexes.erase(indexDelete);
    platformIndexes.erase(indexDelete);
    wallIndexes.erase(indexDelete);
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
    hitBoxes.erase(indexDelete);
    lifePoints.erase(indexDelete);
}

void Galaxy::removePlanetWall(size_t indexDelete) {
    for (auto it = walls.begin(); it != walls.end(); ++it) {
        if (it->index == indexDelete) {
            walls.erase(it);
            break;
        }
    }
    startLayers.erase(indexDelete);
    endLayers.erase(indexDelete);
    widths.erase(indexDelete);
    angles.erase(indexDelete);
    planetIndexes.erase(indexDelete);
}

void Galaxy::removePlanetEntry(size_t indexDelete) {
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        if (it->index == indexDelete) {
            entries.erase(it);
            break;
        }
    }
    startLayers.erase(indexDelete);
    widths.erase(indexDelete);
    angles.erase(indexDelete);
    planetIndexes.erase(indexDelete);
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

void Galaxy::adjustCameraPosition() {
    Vec2 playerPosition = positions[0];

    float offsetDistance = 50.0f;
    float direction = directions[0].value;
    float angleRad = angles[0].deg * M_PI / 180.0f;

    float offsetX = offsetDistance * direction * cos(angleRad);
    float offsetY = offsetDistance * direction * sin(angleRad);

    float targetX = playerPosition.x + offsetX;
    float targetY = playerPosition.y + offsetY;

    float dist = std::sqrt(distSquared(cameraPosition, Vec2{targetX, targetY}));

    float t = 0.01f;

    cameraPosition.x += (targetX - cameraPosition.x) * t * dist / 30.0f;
    cameraPosition.y += (targetY - cameraPosition.y) * t * dist / 30.0f;
}

void Galaxy::draw(Draw& draw){

    draw.clearScreen(0, 0, 0, 50); 

    for (size_t i = 0; i < planets.size(); i++) {
        size_t index = planets[i].index;
        Radius planetRadius = radii[index];
        Vec2 planetPosition = positions[index];

        int drawX = (positions[index].x - cameraPosition.x) * scale + screenWidth / 2; 
        int drawY = (positions[index].y - cameraPosition.y) * scale + screenHeight / 2;

        //rotatePointAroundOrigin(drawX, drawY, angles[0].deg, screenWidth / 2 + 100, screenHeight / 2);
        if(!isCircleVisible(drawX, drawY, planetRadius.value * scale)) continue;
        draw.drawFilledCircle(drawX, drawY, planetRadius.value * scale, 60, 40, 40, 255);
        for(int i = 0; i < layers[index].size(); i++){
            float layerRadius = layers[index][i].value;
            if(!isCircleVisible(drawX, drawY, layerRadius * scale)) continue;
            if(i % 2 == 0){
                draw.drawFilledCircle(drawX, drawY, layerRadius * scale, 40, 20, 20, 255);
            }
            else{
                draw.drawFilledCircle(drawX, drawY, layerRadius * scale, 60, 40, 40, 255);
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

        int drawX = (planetX - cameraPosition.x) * scale + screenWidth / 2.0f;
        int drawY = (planetY - cameraPosition.y) * scale + screenHeight / 2.0f;
        //rotatePointAroundOrigin(drawX, drawY, angles[0].deg, screenWidth / 2 + 100, screenHeight / 2);

        if(!isCircleVisible(drawX, drawY, outerRadius * scale)) continue;

        draw.drawPlatform(drawX, drawY, innerRadius * scale, outerRadius * scale, startAngle, endAngle, 60, 40, 40, 255);
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

        int drawX = (planetX - cameraPosition.x) * scale + screenWidth / 2.0f;
        int drawY = (planetY - cameraPosition.y) * scale + screenHeight / 2.0f;
        //rotatePointAroundOrigin(drawX, drawY, angles[0].deg, screenWidth / 2 + 100, screenHeight / 2);

        if(!isCircleVisible(drawX, drawY, outerRadius * scale)) continue;

        draw.drawPlatform(drawX, drawY, innerRadius * scale, outerRadius * scale, startAngle, endAngle, 40, 20, 20, 255);
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

        int drawX = (planetX - cameraPosition.x) * scale + screenWidth / 2.0f;
        int drawY = (planetY - cameraPosition.y) * scale + screenHeight / 2.0f;

        if(!isCircleVisible(drawX, drawY, outerRadius * scale)) continue;

        //rotatePointAroundOrigin(drawX, drawY, angles[0].deg, screenWidth / 2, screenHeight / 2);

        draw.drawPlatform(drawX, drawY, innerRadius * scale, outerRadius * scale, startAngle, endAngle, 100, 130, 0, 255);
    }

    for (size_t i = 0; i < entities.size(); i++) {
        size_t index = entities[i].index;
        Size entitySize = sizes[index];
        int pivotX = entitySize.width * scale / 2;  
        int pivotY = entitySize.height * scale; 
        int drawX = (positions[index].x - cameraPosition.x) * scale - entitySize.width * scale / 2 + screenWidth / 2; 
        int drawY = (positions[index].y - cameraPosition.y) * scale -entitySize.height * scale + screenHeight / 2;
        //rotatePointAroundOrigin(drawX, drawY, angles[0].deg, screenWidth / 2 + 100, screenHeight / 2);
        float angleDeg = angles[index].deg;
        draw.drawFilledRotatedRect(drawX, drawY, entitySize.width * scale, entitySize.height * scale, angleDeg, pivotX, pivotY, 100, 50, 255, 255);

        //draw life points
        if(index == 0){
            draw.drawFilledRotatedRect(drawX - entitySize.width * scale + 1.5f * lifePoints[index].value * scale, drawY - 10 * scale, 3 * lifePoints[index].value * scale, 5 * scale, angleDeg, pivotX + entitySize.width * scale - 1.5f * lifePoints[index].value * scale, pivotY + 10 * scale, 255, 255, 255, 255);
        }
    }

    for (size_t i = 0; i < worms.size(); i++) {
        size_t index = worms[i].index;
        std::vector<Radius> wormRadiiVec = wormRadii[index];

        for (size_t j = 0; j < wormRadiiVec.size(); j++) {
            float radius = wormRadiiVec[j].value;
            Vec2 wormPosition = wormPositions[index][j];
            int drawX = (wormPosition.x - cameraPosition.x) * scale + screenWidth / 2;
            int drawY = (wormPosition.y - cameraPosition.y) * scale + screenHeight / 2;
            if(!isCircleVisible(drawX, drawY, radius * scale)) continue;
            draw.drawFilledCircle(drawX, drawY, radius * scale, 100, 50, 0, 255);
        }
    }
}