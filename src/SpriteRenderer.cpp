#include "SpriteRenderer.hpp"

#include <array>

namespace Renderer {
    using namespace Breakout;
    using namespace Resource;

    //
    // TODO fucking tutorial
    //
    // (0,0) is the top left of the quad. When we apply transformations like translations,
    // on this quad, they're transformed from the top-left position of the quad.
    SpriteRenderer::SpriteRenderer(const Shader* aShader)
        :shader{ aShader }
    {
        unsigned int VBO{ 0 };

        // zzzz you know how it goes
        constexpr std::array<float, 24> vertices{
            // pos -- tex
            0.f, 1.f, 0.f, 1.f,
            1.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 0.f,

            0.f, 1.f, 0.f, 1.f,
            1.f, 1.f, 1.f, 1.f,
            1.f, 0.f, 1.f, 0.f,
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(0));
        glBindVertexArray(0);
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // this happened bc didn't pay attention, will get fixed in new games, though
        constexpr std::array<float, 24> verticesBg{
            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 1.0f, 1.0f
        };

        glGenVertexArrays(1, &VAO2);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBg), verticesBg.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(0));
        glBindVertexArray(0);
    }

    SpriteRenderer::~SpriteRenderer()
    {
        glDeleteVertexArrays(1, &VAO);

        glDeleteVertexArrays(1, &VAO2);
    }

    void SpriteRenderer::render(const Player& player,
                                const Ball& ball,
                                const Background& background,
                                const Bricks& bricks,
                                const PowerUps& powerUps)
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO figure out a way to do only one draw call
        shader->use();

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // draw background
        background.sprite.bind();
        shader->setUniformMat4("model", background.model);
        shader->setUniformVec3("spriteColour", background.colour);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // draw bricks
        for(unsigned int i{ 0 }; i < bricks.sprites.size(); ++i) {
            if(!bricks.isDestroyed[i]) {
                bricks.sprites[i].bind();
                shader->setUniformMat4("model", bricks.models[i]);
                shader->setUniformVec3("spriteColour", bricks.colours[i]);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        // draw player
        player.sprite.bind();
        shader->setUniformMat4("model", player.model);
        shader->setUniformVec3("spriteColour", player.colour);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // draw ball
        ball.sprite.bind();
        shader->setUniformMat4("model", ball.model);
        shader->setUniformVec3("spriteColour", ball.colour);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // draw power-ups
        for(std::size_t i{ 0 }; i < powerUps.positions.size(); ++i) {
            if(!powerUps.isDestroyed[i]) {
                powerUps.sprites[i].bind();
                shader->setUniformMat4("model", powerUps.models[i]);
                shader->setUniformVec3("spriteColour", powerUps.colours[i]);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
    }

    // TODO this just appeared out of necessity, which means this class is not ok
    void SpriteRenderer::render(const Shader *aShader,
                                const Texture* texture)
    {
        aShader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO2);
        texture->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
};
