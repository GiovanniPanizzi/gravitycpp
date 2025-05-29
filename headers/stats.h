#pragma once
//cinematics
struct Gravity{
    bool apply;
};
//kinetics
struct Position {
    float x;
    float y;
};

struct Velocity {
    float x;
    float y;
};

struct Acceleration {
    float x;
    float y;
};

struct Angle {
    float deg;
};

//body stats 
struct Radius {
    float value;
};

struct Size {
    int width;
    int height;
};

struct Mass {
    float value;
};

struct Friction {
    float value;
};

struct Direction {
    int value;
};

struct Elasticity {
    float value;
};

struct Stamina {
    int value;
    int maxValue;
};

struct layerEntry {
    int depth;
    Angle angle;
    float width;
};

//spriteSheet
struct SpriteSheet {
    
};