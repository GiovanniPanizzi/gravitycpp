#pragma once
#include <vector>
#include "objects.h"
#include "stats.h"
#include "Draw.h"
#include "globals.h"
#include "utils.h"
#include <any>
#include <unordered_map>
#include "sprites.h"

class Galaxy {
    public: 
    size_t index;
    //objects
    std::vector<Object> planets;
    std::vector<Object> entities;
    std::vector<Object> platforms;
    std::vector<Object> walls;
    std::vector<Object> entries;
    std::vector<Object> worms;
    //general stats
    std::unordered_map<size_t, Vec2> positions;
    std::unordered_map<size_t, Vec2> velocities;
    std::unordered_map<size_t, Vec2> relativeVelocities;
    std::unordered_map<size_t, Vec2> accelerations;
    std::unordered_map<size_t, Gravity> gravities;
    std::unordered_map<size_t, std::vector<Vec2>> wormPositions;
    //body stats
    std::unordered_map<size_t, Size> sizes;
    std::unordered_map<size_t, Radius> radii;
    std::unordered_map<size_t, Angle> angles;
    std::unordered_map<size_t, std::vector<Radius>> wormRadii;
    std::unordered_map<size_t, std::vector<Vec2>> wormVelocities;
    std::unordered_map<size_t, std::vector<Vec2>> wormAccelerations;
    std::unordered_map<size_t, std::vector<int>> wormPlanetIndexes;
    std::unordered_map<size_t, std::vector<float>> wormAngles;
    // planet stats
    std::unordered_map<size_t, Mass> masses;
    std::unordered_map<size_t, Friction> frictions;
    std::unordered_map<size_t, Elasticity> elasticities;
    std::unordered_map<size_t, std::vector<Radius>> layers;
    std::unordered_map<size_t, std::vector<size_t>> planetPlatforms;
    std::unordered_map<size_t, std::vector<size_t>> layerEntries;
    std::unordered_map<size_t, std::vector<size_t>> layerWalls;
    // entities stats
    std::unordered_map<size_t, Direction> directions;
    std::unordered_map<size_t, Stamina> jumpStaminas;
    std::unordered_map<size_t, Stamina> moveStamina;
    std::unordered_map<size_t, Stamina> attackStamina;

    std::unordered_map<size_t, HitBox> hitBoxes;
    std::unordered_map<size_t, Stamina> lifePoints;

    // indexes
    std::unordered_map<size_t, int> planetIndexes;
    std::unordered_map<size_t, int> platformIndexes;
    std::unordered_map<size_t, int> wallIndexes;
    // planet struct stats
    std::unordered_map<size_t, float> angularSpeeds;
    std::unordered_map<size_t, int> startLayers;
    std::unordered_map<size_t, int> endLayers;
    std::unordered_map<size_t, int> widths;
    std::unordered_map<size_t, int> heights;
    // sprites
    std::unordered_map<size_t, AnimatedSprite> animatedSpriteMap;


    Galaxy();
    void addPlanet(Vec2 position, Vec2 velocity, Vec2 acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<Radius> planetLayers);
    void addEntity(Vec2 position, Vec2 velocity, Vec2 acceleration, Size size, Angle angle);
    void addPlanetPlatform(Size size, size_t planetIndex, Angle angle, float angularSpeed);
    void addPlanetWall(size_t planetIndex, int planetStartLayer, int planetEndLayer, int width, Angle angle);
    void addPlanetEntry(size_t planetIndex, int planetStartLayer, int width, Angle angle);
    void addWorm(Vec2 position, std::vector<Radius> radii);
    void removePlanetPlatform(size_t index);
    void removePlanet(size_t index);
    void removeEntity(size_t index);
    void removePlanetWall(size_t index);
    void removePlanetEntry(size_t index);
    void removeWorm(size_t index);
    void adjustCameraPosition();
    void draw(Draw& draw);
};