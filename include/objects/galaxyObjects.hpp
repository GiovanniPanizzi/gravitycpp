#pragma once
#include <cstddef>
#include <vector>
#include "stats.hpp"
#include <SDL.h>

struct Object {
    size_t index;
};

struct Humans {
    std::vector<Object> entities;
    std::vector<Vec2> positions;
    std::vector<Vec2> velocities;
    std::vector<Vec2> accelerations;
    std::vector<Vec2> relativeVelocities;
    std::vector<RectSize> sizes;
    std::vector<Angle> angles;
    std::vector<Stamina> jumpStaminas;
    std::vector<Stamina> moveStaminas;
    std::vector<Stamina> attackStaminas;
    std::vector<HitBox> hitBoxes;
    std::vector<Direction> directions;
    std::vector<int> planetIndexes;
    std::vector<int> platformIndexes;
    std::vector<Stamina> lifePoints;
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
    std::vector<std::vector<LayerSection>> layers;
    std::vector<std::vector<size_t>> planetPlatforms;
    std::vector<SDL_Texture*> textures;
};

struct Platforms {
    std::vector<Object> entities;
    std::vector<AnnularSection> shapes;
    std::vector<int> planetIndexes;
    std::vector<float> angularSpeeds;
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
    std::vector<Vec2> noisePosition;
    std::vector<int> onCeiling;
};