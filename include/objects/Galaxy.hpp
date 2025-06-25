#pragma once
#include "galaxyObjects.hpp"
#include "../classes/generalPurpose/Draw.hpp"
#include "../utilities/globals.hpp"
#include "../utilities/utils.hpp"

class Galaxy {

    public: 
    Planets planets;
    Humans humans;
    Platforms platforms;
    Worms worms;

    Galaxy();

    void addPlanet(Vec2 position, Vec2 velocity, Vec2 acceleration, Radius radius, Mass mass, Friction friction, Elasticity elasticity, std::vector<LayerSection> planetLayers);
    void addEntity(Vec2 position, Vec2 velocity, Vec2 acceleration, RectSize size, Angle angle);
    void addPlanetPlatform(AnnularSection shape, size_t planetIndex, float angularSpeed);
    void addWorm(Vec2 position, std::vector<Radius> radii);

    void removePlanetPlatform(size_t index);
    void removePlanet(size_t index);
    void removeEntity(size_t index);
    void removeWorm(size_t index);

    void adjustCameraPosition();
    void draw(Draw& draw);
};