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

    if(!isRectInCircle({humanRectFuturePosition, humanSize}, humanAngle, {humanSize.width / 2, humanSize.height}, Circle{planetPosition, planetRadius.value + 2.0f})) {
        return false;
    }

    humanAngle = std::atan2(dy, dx) - M_PI / 2;

    humanAngle = normalizeAngle(humanAngle);

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
                Radius {layer.shape.innerRadius.value},
                Radius {layer.shape.outerRadius.value + 2.0f},
                Angle {layer.shape.startAngle},
                Angle {layer.shape.endAngle}
            };

            if(isPointInAnnularSection(humanPosition, planetPosition, annularSection)){
                //if human is already in the planet, block velocity towards planet, apply friction and normalize
                Vec2 perpendicularVelocity = velocityTowardsPoint(planetPosition, humanPosition, humanVelocity);
                Vec2 parallelVelocity = subtract(humanVelocity, perpendicularVelocity);
                perpendicularVelocity = {0.0f, 0.0f};
                parallelVelocity = multiply(parallelVelocity, currentGalaxy.planets.frictions[planetIndex].value);
                humanVelocity = add(parallelVelocity, perpendicularVelocity);
                //normalize position into outerRadius
                Vec2 distance = subtract(humanPosition, planetPosition);
                float distLen = length(distance);
                distance = normalize(distance);
                humanPosition = add(multiply(distance, outerRadius), planetPosition);

                return true;
            }

            float travelDistance = -1.0f;

            bool foundIntersection = false;

            /*if(!isRectInCircle({humanRectFuturePosition, humanSize}, humanAngle, {humanSize.width / 2, humanSize.height}, Circle{planetPosition, outerRadius})){
                continue;
            }*/

            for (auto& point : humanPoints) {
                Vec2 rotatedPoint = rotatePoint(point, humanPosition, humanAngle);
                Vec2 futurePoint = add(rotatedPoint, humanVelocity);

                auto intersectionDistance = segmentAnnularSectionIntersection(
                    rotatedPoint, futurePoint, planetPosition,
                    innerRadius, outerRadius, startAngle, endAngle
                );

                if (intersectionDistance.has_value()) {
                    float dist = intersectionDistance.value();
                    if (dist < travelDistance || travelDistance < 0.0f) {
                        travelDistance = dist;
                        foundIntersection = true;
                    }
                }
            }
        }
    }
    return false;
}



void Collider::updateHumanCollisions(Galaxy& currentGalaxy, size_t humanIndex){
    currentGalaxy.humans.planetIndexes[humanIndex] = -1;
    for(size_t i = 0; i < currentGalaxy.planets.entities.size(); i++) {
        if(humanInPlanet(currentGalaxy, humanIndex, i)) {
            currentGalaxy.humans.planetIndexes[humanIndex] = i;
            return;
        }
    }
}

void Collider::updateHumansCollisions(Galaxy& currentGalaxy) {
    for(size_t i = 0; i < currentGalaxy.humans.entities.size(); i++) {
        updateHumanCollisions(currentGalaxy, i);
    }
}
