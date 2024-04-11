#pragma once

#include "pch.hpp"

#include "Texture.hpp"

namespace Breakout {

    // TODO mehhhh, this sucks, but its gonna get fixed in next versions
    struct Player final {
        glm::mat4 model;
        glm::vec3 colour;
        glm::vec2 position;
        glm::vec2 size;
        glm::vec2 velocity;
        Resource::DrawableTexture sprite;
        float acceleration;
        float deceleration;
        unsigned int lives;
        bool isMovingLeft;
        bool isMovingRight;
    };

};
