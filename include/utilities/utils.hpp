#pragma once
#include <math.h>
#include <optional>
#include "generalStructs.hpp"
#include "../objects/stats.hpp"

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

float normalizeAngle(float angleRad);

bool isRectInCircle(const Rect& rect, float angleRad, const Vec2& pivot, const Circle& circle);

Vec2 projectOntoAnnularSectionSurface(const Vec2& point, const AnnularSection& section, const Vec2& center);

bool isPointInAnnularSection(Vec2 point, Vec2 annularSectionCenter, const AnnularSection& section);

bool angleInRange(float angle, float start, float end);

Vec2 rotatePoint(const Vec2& point, const Vec2& pivot, float angleRad);

Vec2 velocityTowardsAnnularSection(
    const Vec2& origin,
    const Vec2& velocity,
    const AnnularSection& section,
    const Vec2& center
);

std::optional<float> segmentAnnularSectionIntersection(
    Vec2 A, Vec2 B, Vec2 C, // segmento AB, centro C
    float innerR, float outerR,
    float startAngle, float endAngle
);


