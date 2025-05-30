#pragma once
#include <vector>
#include "objects.h"
#include "stats.h"
#include "Draw.h"
#include "globals.h"
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
    //general stats
    std::unordered_map<size_t, Position> positions;
    std::unordered_map<size_t, Velocity> velocities;
    std::unordered_map<size_t, Velocity> relativeVelocities;
    std::unordered_map<size_t, Acceleration> accelerations;
    std::unordered_map<size_t, Gravity> gravities;
    //body stats
    std::unordered_map<size_t, Size> sizes;
    std::unordered_map<size_t, Radius> radii;
    std::unordered_map<size_t, Angle> angles;
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


    Galaxy();
    void addPlanet(Position position, Velocity velocity, Acceleration acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<Radius> planetLayers);
    void addEntity(Position position, Velocity velocity, Acceleration acceleration, Size size, Angle angle);
    void addPlanetPlatform(Size size, size_t planetIndex, Angle angle, float angularSpeed);
    void addPlanetWall(size_t planetIndex, int planetStartLayer, int planetEndLayer, int width, Angle angle);
    void addPlanetEntry(size_t planetIndex, int planetStartLayer, int width, Angle angle);
    void removePlanetPlatform(size_t index);
    void removePlanet(size_t index);
    void removeEntity(size_t index);
    void draw(Draw& draw);
};