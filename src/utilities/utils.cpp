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

Vec2 subtract(const Vec2 a, const Vec2 b) {
    return {a.x - b.x, a.y - b.y};
}

Vec2 add(const Vec2 a, const Vec2 b) {
    return {a.x + b.x, a.y + b.y};
}

float dotProduct(const Vec2 a, const Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float length(const Vec2 a) {
    return sqrtf(a.x * a.x + a.y * a.y);
}

Vec2 normalize(const Vec2 a) {
    float len = length(a);
    if (len == 0) return {0, 0};
    return {a.x / len, a.y / len};
}

bool isRectInCircle(const Rect& rect, float angleRad, const Vec2& pivot, const Circle& circle) {
    // Centro rettangolo (utile per il test senza pivot)
    float rectHalfWidth = rect.size.width * 0.5f;
    float rectHalfHeight = rect.size.height * 0.5f;

    // Calcola posizione del cerchio rispetto al pivot
    float relCircleX = circle.position.x - pivot.x;
    float relCircleY = circle.position.y - pivot.y;

    // Ruota il cerchio di -angleRad attorno al pivot (in senso orario)
    float cosA = cosf(-angleRad);
    float sinA = sinf(-angleRad);

    float rotatedCircleX = relCircleX * cosA - relCircleY * sinA;
    float rotatedCircleY = relCircleX * sinA + relCircleY * cosA;

    // Calcola posizione rettangolo rispetto al pivot
    float rectRelX = rect.position.x - pivot.x;
    float rectRelY = rect.position.y - pivot.y;

    // Calcola centro rettangolo rispetto al pivot
    float rectCenterX = rectRelX + rectHalfWidth;
    float rectCenterY = rectRelY + rectHalfHeight;

    // Distanza assoluta del cerchio ruotato rispetto al centro rettangolo ruotato
    float circleDistanceX = fabsf(rotatedCircleX - rectCenterX);
    float circleDistanceY = fabsf(rotatedCircleY - rectCenterY);

    if (circleDistanceX > (rectHalfWidth + circle.radius.value)) return false;
    if (circleDistanceY > (rectHalfHeight + circle.radius.value)) return false;

    if (circleDistanceX <= rectHalfWidth) return true;
    if (circleDistanceY <= rectHalfHeight) return true;

    float cornerDistanceSq = (circleDistanceX - rectHalfWidth) * (circleDistanceX - rectHalfWidth) +
                             (circleDistanceY - rectHalfHeight) * (circleDistanceY - rectHalfHeight);

    return cornerDistanceSq <= (circle.radius.value * circle.radius.value);
}