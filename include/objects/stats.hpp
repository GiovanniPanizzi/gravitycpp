#pragma once
#include "../utilities/generalStructs.hpp"

//cinematics
struct Gravity{
    bool apply;
};

//kinetics
struct Angle {
    float rad;
};

//body stats 
struct Radius {
    float value;
};

struct Mass {
    float value;
};

struct Friction {
    float value;
};

struct Direction {
    float value;
};

struct Elasticity {
    float value;
};

struct Stamina {
    int value;
    int maxValue;
};

struct Color {
    int r;
    int g;
    int b;
    int a;
};

enum class Material {
    VOID,
    ROCK,
    GRAVITANIUM,
    ICE,
    METAL
};

struct AnnularSection {
    Radius innerRadius;
    Radius outerRadius;
    Angle startAngle;
    Angle endAngle;
};

struct LayerSection {
    AnnularSection shape;
    Material material; 
};

struct HitBox {
    Vec2 position;
    float radius;
};