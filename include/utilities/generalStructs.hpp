#pragma once
struct Vec2 {
    float x;
    float y;
};

struct RectSize {
    float width;
    float height;
};

struct Radius {
    float value;
};

struct Rect {
    Vec2 position; // top-left corner
    RectSize size;
};

struct Circle {
    Vec2 position; // center
    Radius radius;
};