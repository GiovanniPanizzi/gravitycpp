#include "../headers/Galaxy.h"
#include <string>
#include <iostream>

Galaxy::Galaxy(){
    Object player;
    player.index = humans.entities.size(); 
    humans.entities.push_back(player);
    humans.positions.push_back(Vec2{0.0f, 0.0f});
    humans.velocities.push_back(Vec2{0.0f, 0.0f});
    humans.relativeVelocities.push_back(Vec2{0, 0});
    humans.accelerations.push_back(Vec2{0.0f, 0.0f});
    humans.sizes.push_back(Size{20, 45});
    humans.angles.push_back(Angle{0.0f});
    humans.planetIndexes.push_back(-1);
    humans.platformIndexes.push_back(-1);
    humans.wallIndexes.push_back(-1);
    humans.jumpStaminas.push_back({50, 50}); 
    humans.moveStaminas.push_back({1, 1});
    humans.attackStaminas.push_back({20, 20});
    humans.directions.push_back({1});
    humans.hitBoxes.push_back({{0.0f, 0.0f}, 40.0f});
    humans.lifePoints.push_back({10, 10});
}

void Galaxy::addPlanet(Vec2 position, Vec2 velocity, Vec2 acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<Radius> planetLayers) {
    size_t index = planets.entities.size();
    planets.entities.push_back({index});
    planets.positions.push_back(position);
    planets.velocities.push_back(velocity);
    planets.accelerations.push_back(acceleration);
    planets.radii.push_back(radius);
    planets.masses.push_back(mass);
    planets.frictions.push_back(friction);
    planets.elasticities.push_back(elasticity);
    planets.angles.push_back({0.0f});
    planets.layers.push_back(planetLayers);
    planets.planetPlatforms.push_back({});
    planets.layerEntries.push_back({});
    planets.layerWalls.push_back({});
}

void Galaxy::addEntity(Vec2 position, Vec2 velocity, Vec2 acceleration, Size size, Angle angle) {
    size_t index = humans.entities.size(); 
    humans.entities.push_back({index});  
    humans.positions.push_back(position);
    humans.velocities.push_back(velocity);
    humans.relativeVelocities.push_back(Vec2{0, 0}); 
    humans.accelerations.push_back(acceleration); 
    humans.sizes.push_back(size); 
    humans.angles.push_back(angle); 
    humans.planetIndexes.push_back(-1);
    humans.platformIndexes.push_back(-1); 
    humans.wallIndexes.push_back(-1); 
    humans.jumpStaminas.push_back({30, 30}); 
    humans.moveStaminas.push_back({5, 5}); 
    humans.attackStaminas.push_back({20, 20});
    humans.animatedSprites.push_back({nullptr, 0, false});
    humans.hitBoxes.push_back({{0.0f, 0.0f}, 40.0f}); 
    humans.lifePoints.push_back({10, 10}); 
}

void Galaxy::addPlanetPlatform(Size size, size_t planetIndex, Angle angle, float angularSpeed) {
    size_t index = platforms.entities.size();
    platforms.entities.push_back({index});
    platforms.planetIndexes.push_back(planetIndex);
    platforms.sizes.push_back(size);
    platforms.angles.push_back(angle);
    platforms.angularSpeeds.push_back(angularSpeed);
    planets.planetPlatforms[planetIndex].push_back(index);
}

void Galaxy::addPlanetWall(size_t planetIndex, int planetStartLayer, int planetEndLayer, int width, Angle angle) {
    size_t index = walls.entities.size();
    walls.entities.push_back({index});
    walls.planetIndexes.push_back(planetIndex);
    walls.startLayers.push_back(planetStartLayer);
    walls.endLayers.push_back(planetEndLayer);
    walls.widths.push_back(width);
    walls.angles.push_back(angle);
    planets.layerWalls[planetIndex].push_back(index);
}

void Galaxy::addPlanetEntry(size_t planetIndex, int planetStartLayer, int width, Angle angle) {
    size_t index = entries.entities.size();
    entries.entities.push_back({index});
    entries.planetIndexes.push_back(planetIndex);
    entries.startLayers.push_back(planetStartLayer);
    entries.widths.push_back(width);
    entries.angles.push_back(angle);
    planets.layerEntries[planetIndex].push_back(index);
}

void Galaxy::addWorm(Vec2 position, std::vector<Radius> wormRadii) {
    size_t wormIndex = worms.entities.size(); 
    worms.entities.push_back({wormIndex});  

    // Aggiungi la posizione iniziale del verme
    worms.positions.push_back(std::vector<Vec2>());
    worms.velocities.push_back(std::vector<Vec2>());
    worms.relativeVelocities.push_back(std::vector<Vec2>());
    worms.accelerations.push_back(std::vector<Vec2>());
    worms.radii.push_back(std::vector<Radius>());
    worms.planetIndexes.push_back(std::vector<int>());
    worms.angles.push_back(std::vector<float>());

    worms.positions[wormIndex].push_back(position);

    // Inizializza i segmenti del verme in un singolo ciclo
    for (size_t i = 0; i < wormRadii.size(); i++) {
        worms.radii[wormIndex].push_back(wormRadii[i]);
        if (i > 0) {
            Vec2 prevPosition = worms.positions[wormIndex][i - 1];
            worms.positions[wormIndex].push_back(Vec2{prevPosition.x + wormRadii[i - 1].value, prevPosition.y});
            std::cout << "Adding worm segment at position: " << worms.positions[wormIndex][i].x << ", " << worms.positions[wormIndex][i].y << std::endl;
        }
        worms.velocities[wormIndex].push_back(Vec2{0.0f, 0.0f});
        worms.relativeVelocities[wormIndex].push_back(Vec2{0.0f, 0.0f});
        worms.accelerations[wormIndex].push_back(Vec2{0.0f, 0.0f});
        worms.planetIndexes[wormIndex].push_back(-1);
        worms.angles[wormIndex].push_back(0.0f);
    }
    worms.lifePoints.push_back({5, 5});
    worms.hitBoxes.push_back({{0.0f, 0.0f}, 30.0f});
}

void Galaxy::removePlanetPlatform(size_t indexDelete) {
    size_t lastIndex = planets.planetPlatforms.size() - 1;

    if (indexDelete != lastIndex) {
        platforms.sizes[indexDelete] = platforms.sizes[lastIndex];
        platforms.angles[indexDelete] = platforms.angles[lastIndex];
        platforms.angularSpeeds[indexDelete] = platforms.angularSpeeds[lastIndex];
    }

    platforms.sizes.pop_back();
    platforms.angles.pop_back();
    platforms.angularSpeeds.pop_back();
}

void Galaxy::removePlanet(size_t indexDelete) {
    size_t lastIndex = planets.entities.size() - 1;

    if (indexDelete != lastIndex) {
        // Sostituisci l'elemento da eliminare con l'ultimo elemento
        planets.entities[indexDelete] = planets.entities[lastIndex];
        planets.positions[indexDelete] = planets.positions[lastIndex];
        planets.velocities[indexDelete] = planets.velocities[lastIndex];
        planets.accelerations[indexDelete] = planets.accelerations[lastIndex];
        planets.radii[indexDelete] = planets.radii[lastIndex];
        planets.masses[indexDelete] = planets.masses[lastIndex];
        planets.frictions[indexDelete] = planets.frictions[lastIndex];
        planets.elasticities[indexDelete] = planets.elasticities[lastIndex];
        planets.angles[indexDelete] = planets.angles[lastIndex];
        planets.layers[indexDelete] = planets.layers[lastIndex];
        planets.planetPlatforms[indexDelete] = planets.planetPlatforms[lastIndex];
        planets.layerEntries[indexDelete] = planets.layerEntries[lastIndex];
        planets.layerWalls[indexDelete] = planets.layerWalls[lastIndex];

        // Aggiorna i riferimenti degli oggetti spostati
        for (size_t& platformIndex : planets.planetPlatforms[indexDelete]) {
            platformIndex = indexDelete;
        }
        for (size_t& entryIndex : planets.layerEntries[indexDelete]) {
            entryIndex = indexDelete;
        }
        for (size_t& wallIndex : planets.layerWalls[indexDelete]) {
            wallIndex = indexDelete;
        }
    }

    planets.entities.pop_back();
    planets.positions.pop_back();
    planets.velocities.pop_back();
    planets.accelerations.pop_back();
    planets.radii.pop_back();
    planets.masses.pop_back();
    planets.frictions.pop_back();
    planets.elasticities.pop_back();
    planets.angles.pop_back();
    planets.layers.pop_back();
    planets.planetPlatforms.pop_back();
    planets.layerEntries.pop_back();
    planets.layerWalls.pop_back();


    for (size_t i = 0; i < humans.planetIndexes.size(); ++i) {
        if (humans.planetIndexes[i] == static_cast<int>(lastIndex)) {
            humans.planetIndexes[i] = static_cast<int>(indexDelete);
        } else if (humans.planetIndexes[i] == static_cast<int>(indexDelete)) {
            humans.planetIndexes[i] = -1; 
        }
    }
}

void Galaxy::removeEntity(size_t indexDelete) {
    size_t lastIndex = humans.entities.size() - 1;

    if (indexDelete != lastIndex) {
        humans.entities[indexDelete] = humans.entities[lastIndex];
        humans.positions[indexDelete] = humans.positions[lastIndex];
        humans.velocities[indexDelete] = humans.velocities[lastIndex];
        humans.relativeVelocities[indexDelete] = humans.relativeVelocities[lastIndex];
        humans.accelerations[indexDelete] = humans.accelerations[lastIndex];
        humans.sizes[indexDelete] = humans.sizes[lastIndex];
        humans.angles[indexDelete] = humans.angles[lastIndex];
        humans.planetIndexes[indexDelete] = humans.planetIndexes[lastIndex];
        humans.platformIndexes[indexDelete] = humans.platformIndexes[lastIndex];
        humans.jumpStaminas[indexDelete] = humans.jumpStaminas[lastIndex];
        humans.moveStaminas[indexDelete] = humans.moveStaminas[lastIndex];
        humans.hitBoxes[indexDelete] = humans.hitBoxes[lastIndex];
        humans.lifePoints[indexDelete] = humans.lifePoints[lastIndex];
    }

    humans.entities.pop_back();
    humans.positions.pop_back();
    humans.velocities.pop_back();
    humans.relativeVelocities.pop_back();
    humans.accelerations.pop_back();
    humans.sizes.pop_back();
    humans.angles.pop_back();
    humans.planetIndexes.pop_back();
    humans.platformIndexes.pop_back();
    humans.jumpStaminas.pop_back();
    humans.moveStaminas.pop_back();
    humans.hitBoxes.pop_back();
    humans.lifePoints.pop_back();
}

void Galaxy::removePlanetWall(size_t wallIndex) {
    size_t lastIndex = planets.layerWalls.size() - 1;

    if (wallIndex != lastIndex) {
        walls.startLayers[wallIndex] = walls.startLayers[lastIndex];
        walls.endLayers[wallIndex] = walls.endLayers[lastIndex];
        walls.widths[wallIndex] = walls.widths[lastIndex];
        walls.angles[wallIndex] = walls.angles[lastIndex];
    }

    walls.startLayers.pop_back();
    walls.endLayers.pop_back();
    walls.widths.pop_back();
    walls.angles.pop_back();
}

void Galaxy::removePlanetEntry(size_t entryIndex) {
    size_t lastIndex = planets.layerEntries.size() - 1;

    if (entryIndex != lastIndex) {
        // Replace the entry to be removed with the last entry
        planets.layerEntries[entryIndex] = planets.layerEntries[lastIndex];
        entries.startLayers[entryIndex] = entries.startLayers[lastIndex];
        entries.widths[entryIndex] = entries.widths[lastIndex];
        entries.angles[entryIndex] = entries.angles[lastIndex];
    }

    // Remove the last entry
    entries.startLayers.pop_back();
    entries.widths.pop_back();
    entries.angles.pop_back();
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
    Vec2 playerPosition = humans.positions[0];

    float offsetDistance = 50.0f;
    float direction = humans.directions[0].value;
    float angleRad = humans.angles[0].deg * M_PI / 180.0f;

    float offsetX = offsetDistance * direction * cos(angleRad);
    float offsetY = offsetDistance * direction * sin(angleRad);

    float targetX = playerPosition.x + offsetX;
    float targetY = playerPosition.y + offsetY;

    float dist = std::sqrt(distSquared(cameraPosition, Vec2{targetX, targetY}));

    float t = 0.01f;

    cameraPosition.x += (targetX - cameraPosition.x) * t * dist / 30.0f;
    cameraPosition.y += (targetY - cameraPosition.y) * t * dist / 30.0f;
}

void Galaxy::draw(Draw& draw) {
    draw.clearScreen(0, 0, 0, 50);

    // Draw planets and their layers
    for (size_t i = 0; i < planets.entities.size(); i++) {
        Vec2 planetPosition = planets.positions[i];
        Radius planetRadius = planets.radii[i];

        int drawX = (planetPosition.x - cameraPosition.x) * scale + screenWidth / 2;
        int drawY = (planetPosition.y - cameraPosition.y) * scale + screenHeight / 2;

        if (!isCircleVisible(drawX, drawY, planetRadius.value * scale)) continue;

        draw.drawFilledCircle(drawX, drawY, planetRadius.value * scale, 60, 40, 40, 255);

        for (size_t j = 0; j < planets.layers[i].size(); j++) {
            float layerRadius = planets.layers[i][j].value;
            if (!isCircleVisible(drawX, drawY, layerRadius * scale)) continue;

            if (j % 2 == 0) {
                draw.drawFilledCircle(drawX, drawY, layerRadius * scale, 40, 20, 20, 255);
            } else {
                draw.drawFilledCircle(drawX, drawY, layerRadius * scale, 60, 40, 40, 255);
            }
        }
    }

    // Draw walls
    for (size_t i = 0; i < planets.entities.size(); i++) {
        for (size_t j = 0; j < planets.layerWalls[i].size(); j++) {
            size_t wallIndex = planets.layerWalls[i][j];

            float angleDeg = walls.angles[wallIndex].deg;
            int startLayer = walls.startLayers[wallIndex];
            int endLayer = walls.endLayers[wallIndex];
            int width = walls.widths[wallIndex];

            Vec2 planetPosition = planets.positions[i];
            float innerRadius = planets.layers[i][startLayer].value;
            float outerRadius = planets.layers[i][endLayer].value;

            float startAngle = angleDeg - width / 2.0f;
            float endAngle = angleDeg + width / 2.0f;

            int drawX = (planetPosition.x - cameraPosition.x) * scale + screenWidth / 2.0f;
            int drawY = (planetPosition.y - cameraPosition.y) * scale + screenHeight / 2.0f;

            if (!isCircleVisible(drawX, drawY, outerRadius * scale)) continue;

            draw.drawPlatform(drawX, drawY, innerRadius * scale, outerRadius * scale, startAngle, endAngle, 60, 40, 40, 255);
        }
    }

    // Draw entries
    for (size_t i = 0; i < planets.entities.size(); i++) {
        for (size_t j = 0; j < planets.layerEntries[i].size(); j++) {
            size_t entryIndex = planets.layerEntries[i][j];

            float angleDeg = entries.angles[entryIndex].deg;
            int startLayer = entries.startLayers[entryIndex];
            int width = entries.widths[entryIndex];

            Vec2 planetPosition = planets.positions[i];
            float innerRadius = planets.layers[i][startLayer].value;
            float outerRadius = (startLayer == 0) ? planets.radii[i].value : planets.layers[i][startLayer - 1].value;

            float startAngle = angleDeg - width / 2.0f;
            float endAngle = angleDeg + width / 2.0f;

            int drawX = (planetPosition.x - cameraPosition.x) * scale + screenWidth / 2.0f;
            int drawY = (planetPosition.y - cameraPosition.y) * scale + screenHeight / 2.0f;

            if (!isCircleVisible(drawX, drawY, outerRadius * scale)) continue;

            draw.drawPlatform(drawX, drawY, innerRadius * scale, outerRadius * scale, startAngle, endAngle, 40, 20, 20, 255);
        }
    }

    // Draw platforms
    for (size_t i = 0; i < planets.entities.size(); i++) {
        for (size_t j = 0; j < planets.planetPlatforms[i].size(); j++) {
            size_t platformIndex = planets.planetPlatforms[i][j];

            float angleDeg = platforms.angles[platformIndex].deg;
            float widthDeg = platforms.sizes[platformIndex].width;
            float height = platforms.sizes[platformIndex].height;

            Vec2 planetPosition = planets.positions[i];
            float radius = planets.radii[i].value;

            float innerRadius = radius;
            float outerRadius = radius + height;

            float startAngle = angleDeg - widthDeg / 2.0f;
            float endAngle = angleDeg + widthDeg / 2.0f;

            int drawX = (planetPosition.x - cameraPosition.x) * scale + screenWidth / 2.0f;
            int drawY = (planetPosition.y - cameraPosition.y) * scale + screenHeight / 2.0f;

            if (!isCircleVisible(drawX, drawY, outerRadius * scale)) continue;

            draw.drawPlatform(drawX, drawY, innerRadius * scale, outerRadius * scale, startAngle, endAngle, 100, 130, 0, 255);
        }
    }

    // Draw entities
    for (size_t i = 0; i < humans.entities.size(); i++) {
        Vec2 entityPosition = humans.positions[i];
        Size entitySize = humans.sizes[i];
        float angleDeg = humans.angles[i].deg;

        int pivotX = entitySize.width * scale / 2;
        int pivotY = entitySize.height * scale;

        int drawX = (entityPosition.x - cameraPosition.x) * scale - entitySize.width * scale / 2 + screenWidth / 2;
        int drawY = (entityPosition.y - cameraPosition.y) * scale - entitySize.height * scale + screenHeight / 2;

        draw.drawFilledRotatedRect(drawX, drawY, entitySize.width * scale, entitySize.height * scale, angleDeg, pivotX, pivotY, 100, 50, 255, 255);

        // Draw life points for the player
        if (i == 0) {
            draw.drawFilledRotatedRect(
                drawX - entitySize.width * scale + 1.5f * humans.lifePoints[i].value * scale,
                drawY - 10 * scale,
                3 * humans.lifePoints[i].value * scale,
                5 * scale,
                angleDeg,
                pivotX + entitySize.width * scale - 1.5f * humans.lifePoints[i].value * scale,
                pivotY + 10 * scale,
                255, 255, 255, 255
            );
        }
    }

    // Draw worms
    for (size_t i = 0; i < worms.entities.size(); i++) {
        for (size_t j = 0; j < worms.radii[i].size(); j++) {
            float radius = worms.radii[i][j].value;
            Vec2 wormPosition = worms.positions[i][j];

            int drawX = (wormPosition.x - cameraPosition.x) * scale + screenWidth / 2;
            int drawY = (wormPosition.y - cameraPosition.y) * scale + screenHeight / 2;

            if (!isCircleVisible(drawX, drawY, radius * scale)) continue;

            draw.drawFilledCircle(drawX, drawY, radius * scale, 100, 50, 0, 255);
        }
    }
}