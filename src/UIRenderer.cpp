#include "UIRenderer.hpp"

#include <sstream>

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {

    static constexpr float UIHeight{ 60.f };

    using namespace Window;
    using namespace Resource;

    UIRenderer::UIRenderer(WindowManager* aWindowManager,
                           const Shader* aShader,
                           TextureManager* aTextureManager,
                           TextRenderer* aTextRenderer)
        :windowManager{ aWindowManager },
         shader{ aShader },
         textureManager{ aTextureManager },
         textRenderer{ aTextRenderer }
    {
        const float vertices[] = {
            0.f, 1.f, 0.f, 1.f, // top-left
            1.f, 0.0f, 1.f, 0.f, // bottom-right
            0.f, 0.0f, 0.f, 0.f, // bottom-left

            0.f, 1.f, 0.f, 1.f, // top-left
            1.f, 1.f, 1.f, 1.f, // top-right
            1.f, 0.0f, 1.f, 0.f, // bottom-right
        };

        unsigned int VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(0));
        glBindVertexArray(0);
    }

    UIRenderer::~UIRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
    }

    void UIRenderer::render(const unsigned int currentLives,
                            const unsigned int currentLevel) const
    {
        shader->use();

        Texture* levelTexture = nullptr;
        if(currentLevel == 0) {
            levelTexture = textureManager->getTexture("LevelOne");
        } else if(currentLevel == 1) {
            levelTexture = textureManager->getTexture("LevelTwo");
        } else if(currentLevel == 2) {
            levelTexture = textureManager->getTexture("LevelThree");
        } else if(currentLevel == 3) {
            levelTexture = textureManager->getTexture("LevelFour");
        }

        static const auto heartTexture = textureManager->getTexture("Heart");
        static const auto groundTexture = textureManager->getTexture("Ground");
        static const int groundHeight{ groundTexture->getHeight() };
        static const int heartWidth{ heartTexture->getWidth() };
        static const int heartHeight{ heartTexture->getHeight() };
        static constexpr float heartStartX{ 10.f };
        static const float levelWidth{ levelTexture->getWidth() };
        static const float levelHeight{ levelTexture->getHeight() };

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // draw ground
        groundTexture->bind();
        glm::mat4 model{ glm::mat4(1.f) };
        model = glm::translate(model, glm::vec3(0.f,
                                                windowManager->getHeightf() - groundHeight,
                                                0.f));
        model = glm::scale(model, glm::vec3(windowManager->getWidthf(), groundHeight * 2.f, 1.0f));
        shader->setUniformMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // draw lives
        heartTexture->bind();
        for(unsigned int i{ 0 }; i < currentLives; ++i) {
            model = glm::mat4(1.f);
            model = glm::translate(model, glm::vec3(heartStartX + (i * heartWidth),
                                                    windowManager->getHeightf() - heartHeight + 5.f,
                                                    0.f));
            model = glm::scale(model, glm::vec3(heartWidth, heartHeight, 1.0f));
            shader->setUniformMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // draw current level
        levelTexture->bind();
        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(windowManager->getWidthf() - levelWidth - 20.f,
                                                windowManager->getHeightf() - levelHeight + 10.f,
                                                0.f));
        model = glm::scale(model, glm::vec3(levelWidth * 0.75f, levelHeight * 0.75f, 1.0f));
        shader->setUniformMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
};
