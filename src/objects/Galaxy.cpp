#include "../../include/objects/Galaxy.hpp"
#include <iostream>

Galaxy::Galaxy(){
    Object player;
    player.index = humans.entities.size(); 
    humans.entities.push_back(player);
    humans.positions.push_back(Vec2{0.0f, 0.0f});
    humans.velocities.push_back(Vec2{0.0f, 0.0f});
    humans.relativeVelocities.push_back(Vec2{0, 0});
    humans.accelerations.push_back(Vec2{0.0f, 0.0f});
    humans.sizes.push_back(RectSize{20, 45});
    humans.angles.push_back(Angle{0.0f});
    humans.planetIndexes.push_back(-1);
    humans.platformIndexes.push_back(-1);
    humans.jumpStaminas.push_back({50, 50}); 
    humans.moveStaminas.push_back({1, 1});
    humans.attackStaminas.push_back({20, 20});
    humans.directions.push_back({1});
    humans.hitBoxes.push_back({{0.0f, 0.0f}, 40.0f});
    humans.lifePoints.push_back({10, 10});
}

void Galaxy::addPlanet(Vec2 position, Vec2 velocity, Vec2 acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<LayerSection> planetLayers) {
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
    planets.textures.push_back(nullptr);
}

void Galaxy::addEntity(Vec2 position, Vec2 velocity, Vec2 acceleration, RectSize size, Angle angle) {
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
    humans.jumpStaminas.push_back({30, 30}); 
    humans.moveStaminas.push_back({5, 5}); 
    humans.attackStaminas.push_back({20, 20});
    humans.hitBoxes.push_back({{0.0f, 0.0f}, 40.0f}); 
    humans.lifePoints.push_back({10, 10}); 
}

void Galaxy::addPlanetPlatform(AnnularSection shape, size_t planetIndex, float angularSpeed) {
    size_t index = platforms.entities.size();
    platforms.entities.push_back({index});
    platforms.planetIndexes.push_back(planetIndex);
    platforms.shapes.push_back(shape);
    platforms.angularSpeeds.push_back(angularSpeed);
    planets.planetPlatforms[planetIndex].push_back(index);
}

void Galaxy::addWorm(Vec2 position, std::vector<Radius> wormRadii) {
    size_t wormIndex = worms.entities.size(); 
    worms.entities.push_back({wormIndex});  

    worms.positions.push_back(std::vector<Vec2>());
    worms.velocities.push_back(std::vector<Vec2>());
    worms.relativeVelocities.push_back(std::vector<Vec2>());
    worms.accelerations.push_back(std::vector<Vec2>());
    worms.radii.push_back(std::vector<Radius>());
    worms.planetIndexes.push_back(std::vector<int>());
    worms.angles.push_back(std::vector<float>());

    worms.positions[wormIndex].push_back(position);

    for (size_t i = 0; i < wormRadii.size(); i++) {
        worms.radii[wormIndex].push_back(wormRadii[i]);
        if (i > 0) {
            Vec2 prevPosition = worms.positions[wormIndex][i - 1];
            worms.positions[wormIndex].push_back(Vec2{prevPosition.x + wormRadii[i - 1].value, prevPosition.y});
            //std::cout << "Adding worm segment at position: " << worms.positions[wormIndex][i].x << ", " << worms.positions[wormIndex][i].y << std::endl;
        }
        worms.velocities[wormIndex].push_back(Vec2{0.0f, 0.0f});
        worms.relativeVelocities[wormIndex].push_back(Vec2{0.0f, 0.0f});
        worms.accelerations[wormIndex].push_back(Vec2{0.0f, 0.0f});
        worms.planetIndexes[wormIndex].push_back(-1);
        worms.angles[wormIndex].push_back(0.0f);
    }
    worms.noisePosition.push_back(position);
    worms.lifePoints.push_back({5, 5});
    worms.hitBoxes.push_back({{0.0f, 0.0f}, 30.0f});
    worms.onCeiling.push_back(-1);
}

void Galaxy::removePlanetPlatform(size_t indexDelete) {
    size_t lastIndex = planets.planetPlatforms.size() - 1;

    if (indexDelete != lastIndex) {
        platforms.shapes[indexDelete] = platforms.shapes[lastIndex];
        platforms.angularSpeeds[indexDelete] = platforms.angularSpeeds[lastIndex];
    }

    platforms.shapes.pop_back();
    platforms.angularSpeeds.pop_back();
}

void Galaxy::removePlanet(size_t indexDelete) {
    size_t lastIndex = planets.entities.size() - 1;

    if (indexDelete != lastIndex) {
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
        planets.textures[indexDelete] = planets.textures[lastIndex];

        for (size_t& platformIndex : planets.planetPlatforms[indexDelete]) {
            platformIndex = indexDelete;
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
    planets.textures.pop_back();

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

void rotatePointAroundOrigin(int& x, int& y, float angleRad, float pivotX, float pivotY) {

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