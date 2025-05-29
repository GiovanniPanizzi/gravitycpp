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
    //objects
    std::vector<Planet> planets;
    std::vector<Entity> entities;
    std::vector<planetPlatform> platforms;
    std::vector<Planet> hollowPlanets;
    //cinematics stats
    std::unordered_map<size_t, Gravity> gravities;
    //kinetics stats
    std::unordered_map<size_t, Position> positions;
    std::unordered_map<size_t, Velocity> velocities;
    std::unordered_map<size_t, Velocity> relativeVelocities;
    std::unordered_map<size_t, Acceleration> accelerations;
    std::unordered_map<size_t, Angle> angles;
    std::unordered_map<size_t, int> planetIndexes;
    std::unordered_map<size_t, int> platformIndexes;
    std::unordered_map<size_t, int> hollowPlanetIndexes;
    std::unordered_map<size_t, float> angularSpeeds;
    //body stats
    std::unordered_map<size_t, Radius> radii;
    std::unordered_map<size_t, Size> sizes;
    std::unordered_map<size_t, Mass> masses;
    std::unordered_map<size_t, Friction> frictions;
    std::unordered_map<size_t, Direction> directions;
    std::unordered_map<size_t, Elasticity> elasticities;
    std::unordered_map<size_t, Stamina> jumpStaminas;
    std::unordered_map<size_t, Stamina> moveStamina;
    //hollowPlanets stats
    std::unordered_map<size_t, std::vector<Radius> > hollowPlanetsRadii;
    std::unordered_map<size_t, std::vector<layerEntry> > planetsLayerEntries;
    std::unordered_map<size_t, std::vector<layerEntry> > holloPlanetswalls;
    std::unordered_map<size_t, bool> isInsideWall;
    //generalIndex
    size_t index;
    //sprites 
    std::unordered_map<size_t, AnimatedSprite> animatedSpriteMap;

    Galaxy();
    void loadTextures();
    void addPlanet(Position position, Velocity velocity, Acceleration acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity);
    void addHollowPlanet(Position position, Velocity velocity, Acceleration acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<Radius> hollowRadii, std::vector<layerEntry> layerEntries, std::vector<layerEntry> walls);
    void addEntity(Position position, Velocity velocity, Acceleration acceleration, Size size, Angle angle);
    void addPlanetPlatform(Size size, size_t planetIndex, Angle angle, float angularSpeed);
    void removePlanetPlatform(size_t index);
    void removePlanet(size_t index);
    void removeEntity(size_t index);
    void draw(Draw& draw);
};