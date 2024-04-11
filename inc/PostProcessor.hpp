#pragma once

#include "pch.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

namespace Effect {

    constexpr float ShakeTime{ .3f };

    class PostProcessor {
    public:
        PostProcessor(const int aWidth,
                      const int aHeight,
                      const Resource::Shader* aShader);

        ~PostProcessor();

        void beginRender();
        void endRender();
        void render(const float time);

    private:
        bool init();

        const Resource::Shader* shader;
        Resource::Texture texture;
        unsigned int multiSampledFBO;
        unsigned int FBO;
        unsigned int multiSampledRBO;
        unsigned int VAO;
        int width;
        int height;

        // cba
    public:
        bool isConfusing;
        bool isChaotic;
        bool isShaking;
    };

};
