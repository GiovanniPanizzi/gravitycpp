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
float distance(float x1, float y1, float x2, float y2) {
    return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
float distanceSquared(float x1, float y1, float x2, float y2) {
    return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}