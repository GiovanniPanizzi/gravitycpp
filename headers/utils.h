#ifndef UTILS_H
#define UTILS_H
#include "stats.h"
#include <cmath>

float distSquared(const Vec2 a, const Vec2 b);
Vec2 subtract(const Vec2 a, const Vec2 b);
Vec2 sum(const Vec2 a, const Vec2 b);
float dot(const Vec2 a, const Vec2 b);
float lengthSquared(const Vec2 v);
float length(const Vec2 v);
Vec2 normalize(const Vec2 v);
Vec2 multiply(const Vec2 v, float scalar);
Vec2 velocityTowardsPoint(const Vec2 point, const Vec2 origin, const Vec2 velocity);

bool pointInCircle(const Vec2 point, const Vec2 circleCenter, const Radius radius);
bool pointInRect(const Vec2 point, const Vec2 rectPosition, const Size rectSize);

Vec2 rotatePointAroundOrigin(const Vec2 point, const float angleDeg, const Vec2 origin);

Vec2 rotate(Vec2 v, float angle);

#endif