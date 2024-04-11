#pragma once

#include "pch.hpp"

#include "Texture.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace Breakout {

    struct Bricks final {
        std::vector<glm::mat4> models;
        std::vector<glm::vec3> colours;
        std::vector<glm::vec2> positions;
        std::vector<glm::vec2> sizes;
        std::vector<Resource::DrawableTexture> sprites;
        std::vector<bool> isSolid;
        std::vector<bool> isDestroyed;
    };

};
