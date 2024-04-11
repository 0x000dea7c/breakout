#pragma once

#include "pch.hpp"

#include "Texture.hpp"

#include <glm/glm.hpp>

namespace Breakout {

    struct Ball final {
        glm::mat4 model;
        glm::vec3 colour;
        glm::vec2 position;
        glm::vec2 size;
        glm::vec2 velocity;
        glm::vec2 center;
        float radius;
        float rotation;
        Resource::DrawableTexture sprite;
        bool isStuck;
        bool isSticky;
        bool isPassingThrough;
    };

};

