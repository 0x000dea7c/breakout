#pragma once

#include "pch.hpp"

#include "Window.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "TextRenderer.hpp"

namespace Renderer {

    // render lives and current level at the bottom of the screen
    class UIRenderer final {
    public:
        UIRenderer(Window::WindowManager* aWindowManager,
                   const Resource::Shader* aShader,
                   Resource::TextureManager* aTextureManager,
                   Renderer::TextRenderer* aTextRenderer);
        ~UIRenderer();

        void render(const unsigned int currentLives,
                    const unsigned int currentLevel) const;
    private:
        Window::WindowManager* windowManager;
        const Resource::Shader* shader;
        Resource::TextureManager* textureManager;
        Renderer::TextRenderer* textRenderer;
        unsigned int VAO;
    };
};
