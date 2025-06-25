#pragma once
#include <math.h>
#include "generalStructs.hpp"

float calculateCosine(float angle);
float calculateSine(float angle);
float calculateAtan2(float y, float x);

//general vec2 functions
Vec2 subtract(const Vec2 a, const Vec2 b);
Vec2 add(const Vec2 a, const Vec2 b);
float dotProduct(const Vec2 a, const Vec2 b);
float length(const Vec2 a);
Vec2 normalize(const Vec2 a);
Vec2 multiply(const Vec2 a, const float scalar);

//useful functions
float distance(Vec2 a, Vec2 b);
float distSquared(Vec2 a, Vec2 b);

Vec2 velocityTowardsPoint(const Vec2 point, const Vec2 origin, const Vec2 velocity);
bool isPointInRect(const Vec2 point, const Rect rect);
bool isPointInCircle(const Vec2 point, const Circle circle);




