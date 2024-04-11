#include "ParticleEmitter.hpp"

#include <array>

namespace Effect {

    using namespace Resource;

    static constexpr unsigned int NewParticles{ 2 };

    static unsigned int gLastDeadParticle{ 0 }; // gonna do magicz with a global

    ParticleEmitter::ParticleEmitter(const Shader* aShader,
                                     const Texture* aTexture)
        :shader{ aShader },
         texture{ aTexture }
    {
        particles.colours.reserve(MaxNumberOfParticles);
        particles.positions.reserve(MaxNumberOfParticles);
        particles.velocities.reserve(MaxNumberOfParticles);
        particles.lives.reserve(MaxNumberOfParticles);

        for(unsigned int i{ 0 }; i < MaxNumberOfParticles; ++i) {
            particles.colours.emplace_back(1.f);
            particles.positions.emplace_back(0.f);
            particles.velocities.emplace_back(0.f);
            particles.lives.emplace_back(0.f);
        }

        unsigned int VBO{ 0 };

        constexpr std::array<float, 24> particleQuad{
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));

        glBindVertexArray(0);

        glDeleteBuffers(1, &VBO);

        particles.sprite.id = texture->id;
    }

    void ParticleEmitter::updateParticles(const float dt,
                                          const glm::vec2& referencePosition,
                                          const glm::vec2& referenceVelocity,
                                          const glm::vec2& offset)
    {
        // "add" new particles.
        for(unsigned int i{ 0 }; i < NewParticles; ++i) {
            auto deadParticleIndex = getFirstDeadParticleIndex();
            respawnParticle(deadParticleIndex, referencePosition, referenceVelocity, offset);
        }

        // Decrease life per particle, update position and brightness (reduce alpha every frame).
        for(unsigned int i{ 0 }; i < particles.lives.size(); ++i) {
            particles.lives[i] -= dt;
            if(particles.lives[i] > 0.f) {
                particles.positions[i] -= particles.velocities[i] * dt;
                particles.colours[i].a -= 2.5f * dt;
            }
        }
    }

    void ParticleEmitter::render()
    {
        shader->use();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        for(unsigned int i{ 0 }; i < particles.colours.size(); ++i) {
            if(particles.lives[i] > 0.f) {
                shader->setUniformVec4("colour", particles.colours[i]);
                shader->setUniformVec2("offset", particles.positions[i]);
                particles.sprite.bind();
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                glBindVertexArray(0);
            }
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    unsigned int ParticleEmitter::getFirstDeadParticleIndex() const
    {
        for(unsigned int i{ gLastDeadParticle }; i < particles.lives.size(); ++i) {
            if(particles.lives[i] <= 0.f) {
                gLastDeadParticle = i;
                return i;
            }
        }

        // NOTE: no dead particles, scan starting from 0 up to lastDeadParticle. The first time it will
        // return 0. If we get here many times then that means particles live for too long.
        for(unsigned int i{ 0 }; i < gLastDeadParticle; ++i) {
            if(particles.lives[i] <= 0.f) {
                gLastDeadParticle = i;
                return i;
            }
        }

        gLastDeadParticle = 0;
        return 0;
    }

    void ParticleEmitter::respawnParticle(const unsigned int deadParticleIndex,
                                          const glm::vec2& referencePosition,
                                          const glm::vec2& referenceVelocity,
                                          const glm::vec2& offset)
    {
        // Reset particle's attributes. We also give a bit of randomness to its colour, starting
        // from .5 and position.
        const float randomPosition{ ((std::rand() % 100) - 50) / 10.f };

        particles.positions[deadParticleIndex] = referencePosition + randomPosition + offset;
        particles.lives[deadParticleIndex] = 1.f;
        particles.colours[deadParticleIndex] = glm::vec4(.67f, .84f, 0.9f, 1.f);
        particles.velocities[deadParticleIndex] = referenceVelocity * 0.1f;
    }

};
