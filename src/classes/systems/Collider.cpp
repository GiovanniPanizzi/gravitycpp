#include "../../../include/classes/systems/Collider.hpp"
#include "../../../include/utilities/utils.hpp"
#include <iostream>

Collider::Collider() {}
Collider::~Collider() {}

// --- humanInPlanet function ---
bool Collider::humanInPlanet(Galaxy& currentGalaxy, size_t humanIndex, size_t planetIndex) {
    Vec2& humanPosition = currentGalaxy.humans.positions[humanIndex];
    Vec2& planetPosition = currentGalaxy.planets.positions[planetIndex];
    RectSize& humanSize = currentGalaxy.humans.sizes[humanIndex];
    Radius& planetRadius = currentGalaxy.planets.radii[planetIndex];
    Vec2& humanVelocity = currentGalaxy.humans.velocities[humanIndex];
    float& humanAngle = currentGalaxy.humans.angles[humanIndex].rad;
    Vec2 humanFuturePosition = add(humanPosition, humanVelocity);

    Vec2 humanRectPosition = subtract(humanPosition, {- humanSize.width / 2, - humanSize.height});
    Vec2 humanRectFuturePosition = subtract(humanFuturePosition, {humanSize.width / 2, humanSize.height});

    float dx = currentGalaxy.planets.positions[planetIndex].x - currentGalaxy.humans.positions[humanIndex].x;
    float dy = currentGalaxy.planets.positions[planetIndex].y - currentGalaxy.humans.positions[humanIndex].y;
    float distance = sqrt(dx * dx + dy * dy);

    if(!isRectInCircle({humanRectFuturePosition, humanSize}, humanAngle, {humanSize.width / 2, humanSize.height}, Circle{planetPosition, planetRadius.value + 5.0f})) {
        return false;
    }

    humanAngle = std::atan2(dy, dx) - M_PI / 2;

    humanAngle = normalizeAngle(humanAngle);

    currentGalaxy.humans.inPlanet[humanIndex] = true;

    //if planet does't have any layers
    if(currentGalaxy.planets.layers[planetIndex].empty()){
        //if velocity is away from planet, don't operate
        Vec2 toPlanet = normalize(subtract(planetPosition, humanPosition));
        float dotVel = dotProduct(humanVelocity, toPlanet);

        if(dotVel < -1.0f) {
            return false;
        }

        //block velocity towards planet and apply friction
        Vec2 perpendicularVelocity = velocityTowardsPoint(planetPosition, humanPosition, humanVelocity);
        Vec2 parallelVelocity = subtract(humanVelocity, perpendicularVelocity);
        perpendicularVelocity = {0.0f, 0.0f};
        parallelVelocity = multiply(parallelVelocity, currentGalaxy.planets.frictions[planetIndex].value);
        humanVelocity = add(parallelVelocity, perpendicularVelocity);
        //normalize position into planet surface
        Vec2 distance = subtract(humanPosition, planetPosition);
        float distLen = length(distance);
        distance = normalize(distance);
        humanPosition = add(multiply(distance, planetRadius.value), planetPosition);
        return true;
    }
    else {
        Vec2 humanPoints[4] = {
            Vec2{humanPosition.x - humanSize.width / 2, humanPosition.y},
            Vec2{humanPosition.x + humanSize.width / 2, humanPosition.y},
            Vec2{humanPosition.x - humanSize.width / 2, humanPosition.y - humanSize.height},
            Vec2{humanPosition.x + humanSize.width / 2, humanPosition.y - humanSize.height},
        };

        for (auto& layer : currentGalaxy.planets.layers[planetIndex]) {
            float outerRadius = layer.shape.outerRadius.value;
            float innerRadius = layer.shape.innerRadius.value;
            float startAngle = layer.shape.startAngle.rad;
            float endAngle = layer.shape.endAngle.rad;

            AnnularSection annularSection = {
                Radius {innerRadius},
                Radius {outerRadius + 2.0f},
                Angle {layer.shape.startAngle},
                Angle {layer.shape.endAngle}
            };

            if (isPointInAnnularSection(humanPosition, planetPosition, annularSection)) {
                // Already inside section → blocca componente verso pianeta
                Vec2 perpendicularVelocity = velocityTowardsPoint(planetPosition, humanPosition, humanVelocity);
                Vec2 parallelVelocity = subtract(humanVelocity, perpendicularVelocity);
                perpendicularVelocity = {0.0f, 0.0f};
                parallelVelocity = multiply(parallelVelocity, currentGalaxy.planets.frictions[planetIndex].value);
                humanVelocity = add(parallelVelocity, perpendicularVelocity);

                Vec2 distance = subtract(humanPosition, planetPosition);
                distance = normalize(distance);
                humanPosition = add(multiply(distance, outerRadius), planetPosition);
                return true;
            }

            Vec2 collisionStart;
            Vec2 collisionEnd;
            float closestDistance = std::numeric_limits<float>::max();
            Vec2 bestNormal;
            bool collided = false;
            float dot;
            bool innerRadiusCollision = false;
            bool outerRadiusCollision = false;
            bool wallCollision = false;

            for (auto& point : humanPoints) {
                Vec2 rotatedPoint = rotatePoint(point, humanPosition, humanAngle);
                Vec2 futurePoint = add(rotatedPoint, humanVelocity);

                Vec2 d = subtract(futurePoint, rotatedPoint);
                float lengthD = length(d);
                if (lengthD == 0.0f) continue;
                Vec2 direction = normalize(d);

                // === Intersection with outerRadius ===
                {
                    Vec2 rel = subtract(rotatedPoint, planetPosition);
                    float a = dotProduct(d, d);
                    float b = 2 * dotProduct(rel, d);
                    float c = dotProduct(rel, rel) - outerRadius * outerRadius;
                    float discriminant = b * b - 4 * a * c;

                    if (discriminant >= 0.0f) {
                        float sqrtD = sqrt(discriminant);
                        for (int sign : {-1, 1}) {
                            float t = (-b + sign * sqrtD) / (2 * a);
                            if (t >= 0.0f && t <= 1.0f) {
                                Vec2 hit = add(rotatedPoint, multiply(d, t));
                                Vec2 relHit = subtract(hit, planetPosition);
                                float angle = atan2(relHit.y, relHit.x);
                                if (angle < 0) angle += 2 * M_PI;
                                float sa = fmod(startAngle + 2 * M_PI, 2 * M_PI);
                                float ea = fmod(endAngle + 2 * M_PI, 2 * M_PI);
                                bool inArc = (sa < ea) ? (angle >= sa && angle <= ea) : (angle >= sa || angle <= ea);

                                if (inArc) {
                                    float dist = t * lengthD;
                                    if (dist < closestDistance) {
                                        closestDistance = dist;
                                        bestNormal = normalize(relHit);
                                        collided = true;
                                        dot = dotProduct(humanVelocity, bestNormal);
                                        outerRadiusCollision = true;
                                        innerRadiusCollision = false;
                                        wallCollision = false;
                                    }
                                }
                            }
                        }
                    }
                }

                // === Intersection with innerRadius ===
                {
                    Vec2 rel = subtract(rotatedPoint, planetPosition);
                    float a = dotProduct(d, d);
                    float b = 2 * dotProduct(rel, d);
                    float c = dotProduct(rel, rel) - innerRadius * innerRadius;
                    float discriminant = b * b - 4 * a * c;

                    if (discriminant >= 0.0f) {
                        float sqrtD = sqrt(discriminant);
                        for (int sign : {-1, 1}) {
                            float t = (-b + sign * sqrtD) / (2 * a);
                            if (t >= 0.0f && t <= 1.0f) {
                                Vec2 hit = add(rotatedPoint, multiply(d, t));
                                Vec2 relHit = subtract(hit, planetPosition);
                                float angle = atan2(relHit.y, relHit.x);
                                if (angle < 0) angle += 2 * M_PI;
                                float sa = fmod(startAngle + 2 * M_PI, 2 * M_PI);
                                float ea = fmod(endAngle + 2 * M_PI, 2 * M_PI);
                                bool inArc = (sa < ea) ? (angle >= sa && angle <= ea) : (angle >= sa || angle <= ea);

                                if (inArc) {
                                    float dist = t * lengthD;
                                    if (dist < closestDistance) {
                                        closestDistance = dist;
                                        bestNormal = normalize(relHit);
                                        collided = true;
                                        dot = dotProduct(humanVelocity, bestNormal);
                                        outerRadiusCollision = false;
                                        innerRadiusCollision = true;
                                        wallCollision = false;
                                    }
                                }
                            }
                        }
                    }
                }

                // === Intersection with walls ===
                for (float angle : {startAngle, endAngle}) {
                    Vec2 dir = { cos(angle), sin(angle) };

                    Vec2 p1 = add(planetPosition, multiply(dir, innerRadius));
                    Vec2 p2 = add(planetPosition, multiply(dir, outerRadius));

                    Vec2 s1 = subtract(futurePoint, rotatedPoint);
                    Vec2 s2 = subtract(p2, p1);

                    float denom = (-s2.x * s1.y + s1.x * s2.y);
                    if (fabs(denom) < 1e-5f) continue;

                    float s = (-s1.y * (rotatedPoint.x - p1.x) + s1.x * (rotatedPoint.y - p1.y)) / denom;
                    float t = ( s2.x * (rotatedPoint.y - p1.y) - s2.y * (rotatedPoint.x - p1.x)) / denom;

                    if (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f) {
                        Vec2 hit = add(rotatedPoint, multiply(s1, t));
                        float dist = t * lengthD;
                        if (dist < closestDistance) {
                            bestNormal = { -dir.y, dir.x };
                            if (dotProduct(bestNormal, s1) > 0) bestNormal = multiply(bestNormal, -1.0f);
                            closestDistance = dist;
                            collided = true;
                            dot = dotProduct(humanVelocity, bestNormal);
                            outerRadiusCollision = false;
                            innerRadiusCollision = false;
                            wallCollision = true;
                        }
                    }
                }
            }

            if (collided) {
                if(innerRadiusCollision) {
                    if (dot > 0.0f) {
                        Vec2 perp = multiply(bestNormal, dot);
                        Vec2 tang = subtract(humanVelocity, perp);
                        humanVelocity = tang;
                    }
                }
                else{
                    if (dot < 0.0f) {
                        Vec2 perp = multiply(bestNormal, dot);
                        Vec2 tang = subtract(humanVelocity, perp);
                        humanVelocity = tang;
                    }
                }
            }
        }
    }
    return false;
}

bool Collider::humanPlatform(Galaxy& currentGalaxy, size_t humanIndex, size_t platformIndex) {
    Vec2& humanPos = currentGalaxy.humans.positions[humanIndex];
    Vec2& humanVel = currentGalaxy.humans.velocities[humanIndex];
    float& humanAngle = currentGalaxy.humans.angles[humanIndex].rad;

    size_t planetIndex = currentGalaxy.platforms.planetIndexes[platformIndex];
    Vec2& planetPos = currentGalaxy.planets.positions[planetIndex];
    AnnularSection platformShape = {
        {currentGalaxy.platforms.shapes[platformIndex].innerRadius.value - 2.0f},
        {currentGalaxy.platforms.shapes[platformIndex].outerRadius.value + 2.0f},
        currentGalaxy.platforms.shapes[platformIndex].startAngle,
        currentGalaxy.platforms.shapes[platformIndex].endAngle
    };

    float angularSpeed = currentGalaxy.platforms.angularSpeeds[platformIndex];
    float friction = currentGalaxy.planets.frictions[planetIndex].value;

    // Check if human is within or very close to the platform annular section
    if (!isPointInAnnularSection(humanPos, planetPos, platformShape)) {
        return false;
    }

    // Vector from planet center to human
    Vec2 fromCenter = subtract(humanPos, planetPos);
    float currentRadius = length(fromCenter);
    Vec2 normFromCenter = normalize(fromCenter);

    // Snap human position to outer radius of platform to avoid vibration
    float outerRadius = currentGalaxy.platforms.shapes[platformIndex].outerRadius.value;
    if (currentRadius != outerRadius) {
        humanPos = add(planetPos, multiply(normFromCenter, outerRadius));
    }

    // Update human angle (facing perpendicular to radius)
    float dx = planetPos.x - humanPos.x;
    float dy = planetPos.y - humanPos.y;
    humanAngle = atan2(dy, dx) - M_PI / 2;

    // Compute platform tangent direction (perpendicular to radius)
    float angleFromCenter = atan2(humanPos.y - planetPos.y, humanPos.x - planetPos.x);
    Vec2 tangent = { -sin(angleFromCenter), cos(angleFromCenter) };

    // Calculate tangential velocity of platform at outer radius
    Vec2 platformTangentialVelocity = multiply(tangent, angularSpeed * outerRadius);

    // Decompose human velocity into radial and tangential components
    Vec2 radialVelocity = multiply(normFromCenter, dotProduct(humanVel, normFromCenter));
    Vec2 tangentialVelocity = subtract(humanVel, radialVelocity);

    // If human is moving inward (landing), block radial velocity and apply friction on tangential
    if (dotProduct(radialVelocity, normFromCenter) < 0.0f) {
        radialVelocity = {0.0f, 0.0f};
        tangentialVelocity = multiply(tangentialVelocity, friction);
    }
    // Else (jumping outward) allow free radial velocity, friction only on tangential
    else {
        tangentialVelocity = multiply(tangentialVelocity, friction);
    }

    // Final human velocity is sum of processed components plus platform velocity
    humanVel = add(radialVelocity, (tangentialVelocity));

    // Save platform tangential velocity as relative velocity (optional for game logic)
    currentGalaxy.humans.relativeVelocities[humanIndex] = platformTangentialVelocity;

    return true;
}

void Collider::updateHumanCollisions(Galaxy& currentGalaxy, size_t humanIndex){
    currentGalaxy.humans.planetIndexes[humanIndex] = -1;
    currentGalaxy.humans.platformIndexes[humanIndex] = -1;
    currentGalaxy.humans.inPlanet[humanIndex] = false;
    for(size_t i = 0; i < currentGalaxy.platforms.entities.size(); i++) {
        if(humanPlatform(currentGalaxy, humanIndex, i)) {
            currentGalaxy.humans.platformIndexes[humanIndex] = i;
            currentGalaxy.humans.planetIndexes[humanIndex] = currentGalaxy.platforms.planetIndexes[i];
            return;
        }
    }
    for(size_t i = 0; i < currentGalaxy.planets.entities.size(); i++) {
        if(humanInPlanet(currentGalaxy, humanIndex, i)) {
            currentGalaxy.humans.planetIndexes[humanIndex] = i;
            currentGalaxy.humans.relativeVelocities[humanIndex] = {0.0f, 0.0f};
            return;
        }
    }
}

void Collider::updateHumansCollisions(Galaxy& currentGalaxy) {
    for(size_t i = 0; i < currentGalaxy.humans.entities.size(); i++) {
        updateHumanCollisions(currentGalaxy, i);
    }
}
