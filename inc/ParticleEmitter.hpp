#pragma once

#include "pch.hpp"

#include "Texture.hpp"
#include "Shader.hpp"

#include <glm/glm.hpp>
#include <vector>

namespace Effect {

    constexpr unsigned int MaxNumberOfParticles{ 500 };

    struct Particles final {
        std::vector<glm::vec4> colours;
        std::vector<glm::vec2> positions;
        std::vector<glm::vec2> velocities;
        std::vector<float> lives;
        Resource::DrawableTexture sprite; // yes, only one texture for all of them in this case
    };

    class ParticleEmitter final {
    public:
        ParticleEmitter(const Resource::Shader* aShader,
                        const Resource::Texture* aTexture);

        void updateParticles(const float dt,
                             const glm::vec2& referencePosition,
                             const glm::vec2& referenceVelocity,
                             const glm::vec2& offset);

        // TODO aye, no way in hell this class should be drawing stuff
        void render();

    private:

        unsigned int getFirstDeadParticleIndex() const;

        void respawnParticle(const unsigned int deadParticleIndex,
                             const glm::vec2& referencePosition,
                             const glm::vec2& referenceVelocity,
                             const glm::vec2& offset);

        Particles particles;
        const Resource::Shader* shader;
        const Resource::Texture* texture;
        unsigned int VAO;
    };

};
