#pragma once

#include "pch.hpp"

#include "Shader.hpp"
#include "Player.hpp"
#include "Ball.hpp"
#include "Background.hpp"
#include "Bricks.hpp"
#include "PowerUps.hpp"
#include "Texture.hpp"

namespace Renderer {

    class SpriteRenderer final {
    public:
        SpriteRenderer(const Resource::Shader* aShader);
        ~SpriteRenderer();

        void render(const Breakout::Player& player,
                    const Breakout::Ball& ball,
                    const Breakout::Background& background,
                    const Breakout::Bricks& bricks,
                    const Breakout::PowerUps& powerUps);

        void render(const Resource::Shader *aShader,
                    const Resource::Texture* texture);
    private:
        const Resource::Shader* shader;
        unsigned int VAO, VAO2;
    };

};
