#include "../../include/utilities/utils.hpp"

float calculateCosine(float angle) {
    return cosf(angle);
}
float calculateSine(float angle) {
    return sinf(angle);
}
float calculateAtan2(float y, float x) {
    return atan2f(y, x);
}
float distance(Vec2 a, Vec2 b) {
    return sqrtf(powf(b.x - a.x, 2) + powf(b.y - a.y, 2));
}
float distSquared(Vec2 a, Vec2 b) {
    return powf(b.x - a.x, 2) + powf(b.y - a.y, 2);
}