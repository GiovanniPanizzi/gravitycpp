#include "../../include/utilities/utils.hpp"
#include <cmath>
#include <iostream>

// --- Provided Utility Functions (unchanged logic, but updated to use 'shape' if passed LayerSection) ---
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

Vec2 multiply(const Vec2 a, const float scalar) {
    return {a.x * scalar, a.y * scalar};
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

Vec2 velocityTowardsPoint(const Vec2 point, const Vec2 origin, const Vec2 velocity) {
    Vec2 toPoint = subtract(point, origin);
    float dist = length(toPoint);
    if (dist == 0) return {0, 0};

    Vec2 dir = normalize(toPoint);
    float projection = dotProduct(velocity, dir);

    if (projection <= 0) return {0, 0};

    return multiply(dir, projection);
}

bool isRectInCircle(const Rect& rect, float angleRad, const Vec2& pivot, const Circle& circle) {
    float rectHalfWidth = rect.size.width * 0.5f;
    float rectHalfHeight = rect.size.height * 0.5f;

    // Calcola posizione assoluta del pivot
    Vec2 rectTopLeft = rect.position;
    Vec2 pivotWorld = add(rectTopLeft, pivot);

    // Posizione del cerchio rispetto al pivot
    float relCircleX = circle.position.x - pivotWorld.x;
    float relCircleY = circle.position.y - pivotWorld.y;

    // Ruota il cerchio nel sistema locale del rettangolo
    float cosA = cosf(-angleRad);
    float sinA = sinf(-angleRad);

    float rotatedCircleX = relCircleX * cosA - relCircleY * sinA;
    float rotatedCircleY = relCircleX * sinA + relCircleY * cosA;

    // Distanza del centro cerchio rispetto al rettangolo (non ruotato)
    float circleDistanceX = fabsf(rotatedCircleX + pivot.x - rectHalfWidth);
    float circleDistanceY = fabsf(rotatedCircleY + pivot.y - rectHalfHeight);

    if (circleDistanceX > (rectHalfWidth + circle.radius.value)) return false;
    if (circleDistanceY > (rectHalfHeight + circle.radius.value)) return false;

    if (circleDistanceX <= rectHalfWidth) return true;
    if (circleDistanceY <= rectHalfHeight) return true;

    float cornerDistanceSq = (circleDistanceX - rectHalfWidth) * (circleDistanceX - rectHalfWidth) +
                             (circleDistanceY - rectHalfHeight) * (circleDistanceY - rectHalfHeight);

    return cornerDistanceSq <= (circle.radius.value * circle.radius.value);
}

Vec2 rotatePoint(const Vec2& point, const Vec2& pivot, float angleRad) {
    float s = calculateSine(angleRad);
    float c = calculateCosine(angleRad);

    float x = point.x - pivot.x;
    float y = point.y - pivot.y;

    float xNew = x * c - y * s;
    float yNew = x * s + y * c;

    return {xNew + pivot.x, yNew + pivot.y};
}

float normalizeAngle(float angle) {
    const float PI = M_PI;
    const float TWO_PI = 2.0f * M_PI;

    while (angle > PI) angle -= TWO_PI;
    while (angle < -PI) angle += TWO_PI;

    return angle;
}

bool angleInRange(float angle, float start, float end) {
    angle = normalizeAngle(angle);
    start = normalizeAngle(start);
    end = normalizeAngle(end);
    if (start < end)
        return angle >= start && angle <= end;
    else
        return angle >= start || angle <= end;
}

// Updated to use .shape
Vec2 projectOntoAnnularSectionSurface(const Vec2& position, const LayerSection& section, const Vec2& annulusCenter) {
    Vec2 dir = subtract(position, annulusCenter);
    float dist = length(dir);
    if (dist == 0) {
        return add(annulusCenter, {section.shape.outerRadius.value, 0});
    }

    Vec2 dirNormalized = normalize(dir);

    float clampedDist = std::clamp(dist, section.shape.innerRadius.value, section.shape.outerRadius.value);

    return add(annulusCenter, multiply(dirNormalized, clampedDist));
}

// Updated to use .shape
bool isPointInAnnularSection(Vec2 point, Vec2 annularSectionCenter, const AnnularSection& section) {
    float distSq = distSquared(point, annularSectionCenter);
    float dist = sqrtf(distSq);

    if (dist < section.innerRadius.value || dist > section.outerRadius.value) {
        return false;
    }

    float angle = normalizeAngle(calculateAtan2(point.y - annularSectionCenter.y, point.x - annularSectionCenter.x));
    
    if(!angleInRange(angle, section.startAngle.rad, section.endAngle.rad)) {
        return false;
    }

    return true;
};

// Updated to use .shape
Vec2 velocityTowardsAnnularSection(
    const Vec2& origin,
    const Vec2& velocity,
    const AnnularSection& section,
    const Vec2& center
) {
    Vec2 dir = subtract(origin, center);
    float dist = length(dir);

    if (dist == 0.0f) dir = {1.0f, 0.0f};
    else dir = normalize(dir);

    float clampedDist = std::clamp(dist, section.innerRadius.value, section.outerRadius.value);

    float angle = normalizeAngle(calculateAtan2(dir.y, dir.x));

    float clampedAngle = angle;
    if (!angleInRange(angle, section.startAngle.rad, section.endAngle.rad)) {
        float start = normalizeAngle(section.startAngle.rad);
        float end = normalizeAngle(section.endAngle.rad);
            
        float shortestAngularDistToStart = std::fmod(start - angle + M_PI * 3.0f, 2.0f * M_PI) - M_PI;
        float shortestAngularDistToEnd = std::fmod(end - angle + M_PI * 3.0f, 2.0f * M_PI) - M_PI;

        if (std::abs(shortestAngularDistToStart) < std::abs(shortestAngularDistToEnd)) {
            clampedAngle = section.startAngle.rad;
        } else {
            clampedAngle = section.endAngle.rad;
        }
    }

    Vec2 closestPoint = {
        center.x + clampedDist * calculateCosine(clampedAngle),
        center.y + clampedDist * calculateSine(clampedAngle)
    };

    Vec2 toSection = subtract(closestPoint, origin);
    float toSectionLength = length(toSection);
    if (toSectionLength == 0.0f) return {0.0f, 0.0f};

    Vec2 toSectionDir = normalize(toSection);
    float projection = dotProduct(velocity, toSectionDir);

    if (projection <= 0.001f) return {0.0f, 0.0f};

    return multiply(toSectionDir, projection);
}

std::optional<float> segmentAnnularSectionIntersection(
    Vec2 A, Vec2 B, Vec2 C,
    float innerR, float outerR,
    float startAngle, float endAngle
) {
    Vec2 d = subtract(B, A);
    Vec2 f = subtract(A, C);

    auto solveQuadratic = [&](float R) -> std::vector<float> {
        float a = dotProduct(d, d);
        float b = 2 * dotProduct(f, d);
        float c = dotProduct(f, f) - R * R;
        float discriminant = b * b - 4 * a * c;

        std::vector<float> ts;
        if (discriminant >= 0) {
            float sqrtD = sqrt(discriminant);
            float t1 = (-b - sqrtD) / (2 * a);
            float t2 = (-b + sqrtD) / (2 * a);
            if (t1 >= 0.0f && t1 <= 1.0f) ts.push_back(t1);
            if (t2 >= 0.0f && t2 <= 1.0f) ts.push_back(t2);
        }
        return ts;
    };

    auto angleInRange = [&](float angle) {
        if (angle < 0) angle += 2 * M_PI;
        float sa = fmod(startAngle + 2 * M_PI, 2 * M_PI);
        float ea = fmod(endAngle + 2 * M_PI, 2 * M_PI);
        if (sa < ea)
            return angle >= sa && angle <= ea;
        else
            return angle >= sa || angle <= ea;
    };

    std::vector<float> tsOuter = solveQuadratic(outerR);
    std::vector<float> tsInner = solveQuadratic(innerR);

    std::vector<std::pair<float, Vec2>> candidates;

    // Controlla le intersezioni con outerR
    for (float t : tsOuter) {
        Vec2 point = { A.x + t * d.x, A.y + t * d.y };
        float dist = length(subtract(point, C));
        float angle = atan2(point.y - C.y, point.x - C.x);
        if (angle < 0) angle += 2 * M_PI;

        if (dist >= innerR && dist <= outerR && angleInRange(angle)) {
            candidates.emplace_back(t, point);
        }
    }

    // Controlla le intersezioni con innerR
    for (float t : tsInner) {
        Vec2 point = { A.x + t * d.x, A.y + t * d.y };
        float dist = length(subtract(point, C));
        float angle = atan2(point.y - C.y, point.x - C.x);
        if (angle < 0) angle += 2 * M_PI;

        if (dist >= innerR && dist <= outerR && angleInRange(angle)) {
            candidates.emplace_back(t, point);
        }
    }

    if (candidates.empty()) return std::nullopt;

    // Prendi la intersezione con il valore t più piccolo (prima intersezione lungo il segmento)
    auto comp = [](auto& a, auto& b) { return a.first < b.first; };
    auto best = std::min_element(candidates.begin(), candidates.end(), comp);

    return best->first * length(d);
}

