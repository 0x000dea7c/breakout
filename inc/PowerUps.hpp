#pragma once

#include "pch.hpp"

#include "Texture.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace Breakout {

    enum class PowerUpType {
        SPEED,
        STICKY,
        PASSTHROUGH,
        PADSIZEINCREASE,
        CONFUSE,
        CHAOS,
    };

    struct PowerUps final {
        std::vector<glm::mat4> models;
        std::vector<glm::vec3> colours;
        std::vector<glm::vec2> positions;
        std::vector<glm::vec2> velocities;
        std::vector<glm::vec2> sizes;
        std::vector<Resource::DrawableTexture> sprites;
        std::vector<float> durations;
        std::vector<PowerUpType> types;
        std::vector<bool> isDestroyed;
        std::vector<bool> isActive;
    };

};
