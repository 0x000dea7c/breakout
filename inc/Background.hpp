#pragma once

#include "pch.hpp"

#include "Texture.hpp"

#include <glm/glm.hpp>

namespace Breakout {

    struct Background final {
        glm::mat4 model;
        glm::vec3 colour;
        glm::vec2 position;
        glm::vec2 size;
        glm::vec2 velocity;
        Resource::DrawableTexture sprite;
    };

};
