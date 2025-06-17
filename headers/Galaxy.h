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
    
    Planets planets;
    Humans humans;
    Platforms platforms;
    Walls walls;
    Entries entries;
    Worms worms;

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