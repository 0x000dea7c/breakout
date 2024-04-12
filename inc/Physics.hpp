#pragma once

#include "pch.hpp"

#include <glm/glm.hpp>

namespace Physics {

    // used to get out of objects once you collide with them
    struct CollisionData {
        glm::vec2 normal;
        glm::vec2 closestPoint;
        float depth;
    };

    bool circleAABBCollided(const glm::vec2& circleCenter,
                            const float circleRadius,
                            const glm::vec2& aabbSize,
                            const glm::vec2& aabbPosition,
                            CollisionData& out)
    {
        const glm::vec2 aabbMin{ glm::vec2(aabbPosition.x, aabbPosition.y) };
        const glm::vec2 aabbMax{ glm::vec2(aabbPosition.x + aabbSize.x, aabbPosition.y + aabbSize.y) };
        const glm::vec2 L{ glm::clamp(circleCenter, aabbMin, aabbMax)};
        const glm::vec2 closestPoint{ L - circleCenter };
        const float distCircleCenterToClosestPoint{ glm::dot(closestPoint, closestPoint) };
        const float r2{ circleRadius * circleRadius };
        const bool collision = distCircleCenterToClosestPoint < r2;

        if(collision) {
            if(distCircleCenterToClosestPoint != 0.f) {
                // circle center is not inside AABB.
                const float d = sqrtf(distCircleCenterToClosestPoint);
                out.depth = circleRadius - d;
                out.normal = glm::normalize(closestPoint);
                out.closestPoint = circleCenter + (out.normal * d);
            } else {
                // serious trouble, most likely bc ball was too fast, circle center
                // is iNSiDE dA AABB
                const glm::vec2 mid{(aabbMin + aabbMax) * 0.5f};
                const glm::vec2 aabbRadius{ (aabbMax - aabbMin) * 0.5f }; // ye, radius, for me
                const glm::vec2 dFromCircleCenterToAABBCenter{ circleCenter - mid };
                const glm::vec2 dAbs{ glm::normalize(dFromCircleCenterToAABBCenter) };

                const float xOverlap{ aabbRadius.x - dAbs.x };
                const float yOverlap{ aabbRadius.y - dAbs.y };

                if(xOverlap > yOverlap) {
                    out.depth = yOverlap;
                    out.normal = glm::vec2(0.f, 1.f) * ((dFromCircleCenterToAABBCenter.y) < 0.f ? 1.f : -1.f);
                } else {
                    out.depth = xOverlap;
                    out.normal = glm::vec2(1.f, 0.f) * ((dFromCircleCenterToAABBCenter.x) < 0.f ? 1.f : -1.f);
                }

                out.depth += circleRadius;
                out.closestPoint = circleCenter - (out.normal * out.depth);
            }
        }

        return collision;
    }

    // TODO this is most likely incorrect, don't use this as a reference for your games
    bool checkSweptCircleAABBCollision(const glm::vec2& circleCenter,
                                       const float circleRadius,
                                       const glm::vec2& circleVelocity,
                                       const glm::vec2& aabbSize,
                                       const glm::vec2& aabbPosition,
                                       CollisionData& out,
                                       const float deltaTime)
    {
        if(glm::length(circleVelocity) == 0) {
            return false;
        }

        // in this frame it collided already, so return true
        if(circleAABBCollided(circleCenter,
                              circleRadius,
                              aabbSize,
                              aabbPosition,
                              out)) {
            return true;
        }

        static constexpr int numSteps{ 3 }; // to do swept intersection

        const glm::vec2 circleEndPos{ (circleVelocity + circleCenter) * deltaTime };
        const float dist{ glm::length(circleEndPos - circleCenter) };
        const float step{ dist / static_cast<float>(numSteps) };

        for(int i{ 0 }; i < numSteps; ++i) {
            glm::vec2 circleCurrentPos{ circleCenter + (static_cast<float>(i) * step) * (glm::normalize(circleVelocity) * deltaTime) };

            if(circleAABBCollided(circleCurrentPos,
                                  circleRadius,
                                  aabbSize,
                                  aabbPosition,
                                  out)) {
                return true;
            }
        }

        return false;
    }

    bool checkAABBCollision(const glm::vec2& positionA,
                            const glm::vec2& sizeA,
                            const glm::vec2& positionB,
                            const glm::vec2& sizeB)
    {
        const bool collisionX{ positionA.x + sizeA.x >= positionB.x &&
                               positionB.x + sizeB.x >= positionA.x };
        const bool collisionY{ positionA.y + sizeA.y >= positionB.y &&
                               positionB.y + sizeB.y >= positionA.y };

        return collisionX && collisionY;
    }
};
