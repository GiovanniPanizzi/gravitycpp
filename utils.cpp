#include "headers/utils.h"

float distSquared(const Vec2 a, const Vec2 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

Vec2 subtract(const Vec2 a, const Vec2 b) {
    return Vec2({a.x - b.x, a.y - b.y});
}

Vec2 sum(const Vec2 a, const Vec2 b) {
    return Vec2({a.x + b.x, a.y + b.y});
}

float dot(const Vec2 a, const Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float lengthSquared(const Vec2 v) {
    return v.x * v.x + v.y * v.y;
}

float length(const Vec2 v) {
    return std::sqrt(lengthSquared(v));
}

Vec2 normalize(const Vec2 v) {
    float len = length(v);
    if (len == 0) return Vec2({0, 0});
    return Vec2({v.x / len, v.y / len});
}

Vec2 multiply(const Vec2 v, float scalar) {
    return Vec2({v.x * scalar, v.y * scalar});
}

Vec2 velocityTowardsPoint(const Vec2 point, const Vec2 origin, const Vec2 velocity) {
    Vec2 dir = normalize(subtract(point, origin));
    float projLength = dot(velocity, dir);
    return multiply(dir, projLength);
}

bool pointInCircle(const Vec2 point, const Vec2 circleCenter, const Radius radius) {
    float dx = point.x - circleCenter.x;
    float dy = point.y - circleCenter.y;
    return (dx * dx + dy * dy <= radius.value * radius.value);
}

bool pointInRect(const Vec2 point, const Vec2 rectPosition, const Size rectSize) {
    return (point.x >= rectPosition.x && point.x <= rectPosition.x + rectSize.width &&
            point.y >= rectPosition.y && point.y <= rectPosition.y + rectSize.height);
}

Vec2 rotatePointAroundOrigin(const Vec2 point, const float angleDeg, const Vec2 origin) {
    float rad = angleDeg * M_PI / 180.0f;
    float cosAngle = std::cos(rad);
    float sinAngle = std::sin(rad);
    
    float translatedX = point.x - origin.x;
    float translatedY = point.y - origin.y;

    return Vec2({
        translatedX * cosAngle - translatedY * sinAngle + origin.x,
        translatedX * sinAngle + translatedY * cosAngle + origin.y}
    );
}

