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
    if (dist == 0) return {0, 0}; // Nessuna direzione definita

    Vec2 dir = normalize(toPoint);
    float projection = dotProduct(velocity, dir); // Proiezione scalare

    if (projection <= 0) return {0, 0}; // La velocità va dalla parte opposta

    return multiply(dir, projection); // Vettore nella direzione di 'point'
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

// Funzione di supporto per ruotare un punto attorno a pivot di angle rad
Vec2 rotatePoint(const Vec2& point, const Vec2& pivot, float angleRad) {
    float s = sinf(angleRad);
    float c = cosf(angleRad);

    // Trasla punto rispetto al pivot
    float x = point.x - pivot.x;
    float y = point.y - pivot.y;

    // Ruota
    float xNew = x * c - y * s;
    float yNew = x * s + y * c;

    // Ritorna punto ruotato
    return {xNew + pivot.x, yNew + pivot.y};
}

// Normalizza angolo tra 0 e 2*PI
float normalizeAngle(float angle) {
    while (angle < 0) angle += 2 * M_PI;
    while (angle >= 2 * M_PI) angle -= 2 * M_PI;
    return angle;
}

// Verifica se angle è nell'intervallo circolare [start, end]
bool angleInRange(float angle, float start, float end) {
    angle = normalizeAngle(angle);
    start = normalizeAngle(start);
    end = normalizeAngle(end);
    if (start < end)
        return angle >= start && angle <= end;
    else
        return angle >= start || angle <= end; // intervallo che attraversa 0
}

bool isRectInAnnularSection(
    const Rect& rect,
    float rectAngleRad,
    const AnnularSection& section,
    const Vec2& pivot,
    const Vec2& annulusCenter
) {
    Vec2 vertices[4] = {
        rect.position,
        {rect.position.x + rect.size.width, rect.position.y},
        {rect.position.x + rect.size.width, rect.position.y + rect.size.height},
        {rect.position.x, rect.position.y + rect.size.height}
    };

    for (int i = 0; i < 4; i++) {
        // Ruota il vertice attorno al pivot di -rectAngleRad
        Vec2 rotatedVertex = rotatePoint(vertices[i], pivot, -rectAngleRad);

        // Calcola il vettore dal centro dell'annulus al vertice ruotato
        Vec2 dir = {rotatedVertex.x - annulusCenter.x, rotatedVertex.y - annulusCenter.y};
        float dist = sqrtf(dir.x * dir.x + dir.y * dir.y);

        // Verifica che il vertice sia tra raggio interno ed esterno
        if (dist < section.innerRadius.value || dist > section.outerRadius.value) continue;

        // Calcola angolo del vertice rispetto al centro dell'annulus
        float vertexAngle = atan2f(dir.y, dir.x);

        // Verifica se l'angolo è nell'intervallo [startAngle, endAngle]
        if (angleInRange(vertexAngle, section.startAngle.rad, section.endAngle.rad)) {
            return true;
        }
    }

    return false;
}
