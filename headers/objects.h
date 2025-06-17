#pragma once
#include <cstddef>
#include "stats.h"
#include "sprites.h"

struct Object {
    size_t index;
};

struct Humans {
    std::vector<Object> entities;
    std::vector<Vec2> positions;
    std::vector<Vec2> velocities;
    std::vector<Vec2> accelerations;
    std::vector<Vec2> relativeVelocities;
    std::vector<Size> sizes;
    std::vector<Angle> angles;
    std::vector<Stamina> jumpStaminas;
    std::vector<Stamina> moveStaminas;
    std::vector<Stamina> attackStaminas;
    std::vector<HitBox> hitBoxes;
    std::vector<Direction> directions;
    std::vector<int> planetIndexes;
    std::vector<int> platformIndexes;
    std::vector<int> wallIndexes;
    std::vector<Stamina> lifePoints;
    std::vector<AnimatedSprite> animatedSprites;
};

struct Planets {
    std::vector<Object> entities;
    std::vector<Vec2> positions;
    std::vector<Vec2> velocities;
    std::vector<Vec2> accelerations;
    std::vector<Radius> radii;
    std::vector<Mass> masses;
    std::vector<Friction> frictions;
    std::vector<Elasticity> elasticities;
    std::vector<Angle> angles;
    std::vector<float> angularSpeeds;
    std::vector<std::vector<Radius>> layers;
    std::vector<std::vector<size_t>> planetPlatforms;
    std::vector<std::vector<size_t>> layerEntries;
    std::vector<std::vector<size_t>> layerWalls;
};

struct Platforms {
    std::vector<Object> entities;
    std::vector<Size> sizes;
    std::vector<Angle> angles;
    std::vector<int> planetIndexes;
    std::vector<float> angularSpeeds;
};

struct Walls {
    std::vector<Object> entities;
    std::vector<int> startLayers;
    std::vector<int> endLayers;
    std::vector<int> widths;
    std::vector<Angle> angles;
    std::vector<int> planetIndexes;
};

struct Entries {
    std::vector<Object> entities;
    std::vector<int> startLayers;
    std::vector<int> endLayers;
    std::vector<int> widths;
    std::vector<Angle> angles;
    std::vector<int> planetIndexes;
};

struct Worms {
    std::vector<Object> entities;
    std::vector<std::vector<Vec2>> positions;
    std::vector<std::vector<Radius>> radii;
    std::vector<std::vector<Vec2>> velocities;
    std::vector<std::vector<Vec2>> relativeVelocities;
    std::vector<std::vector<Vec2>> accelerations;
    std::vector<std::vector<int>> planetIndexes;
    std::vector<std::vector<float>> angles;
    std::vector<Stamina> lifePoints;
    std::vector<HitBox> hitBoxes;
};