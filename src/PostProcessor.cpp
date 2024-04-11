#include "PostProcessor.hpp"

#include <cstdlib>

namespace Effect {

    using namespace Resource;

    static constexpr int RenderbufferSamples{ 4 };

    PostProcessor::PostProcessor(const int aWidth,
                                 const int aHeight,
                                 const Shader* aShader)
        :shader{ aShader },
         width{ aWidth },
         height{ aHeight },
         isConfusing{ false },
         isChaotic{ false },
         isShaking{ false }

    {
        if(!init()) {
            std::cerr << "Couldn't initialize post processor.\n";
            exit(EXIT_FAILURE);
        }
    }

    PostProcessor::~PostProcessor()
    {
        glDeleteVertexArrays(1, &VAO);
    }

    bool PostProcessor::init()
    {
        texture.generate(width,
                         height,
                         GL_RGB,
                         GL_RGB,
                         GL_REPEAT,
                         GL_REPEAT,
                         GL_LINEAR,
                         GL_LINEAR,
                         nullptr);

        // initialize renderbuffer, framebuffers.
        glGenFramebuffers(1, &multiSampledFBO);
        glGenFramebuffers(1, &FBO);
        glGenRenderbuffers(1, &multiSampledRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, multiSampledFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, multiSampledRBO);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, RenderbufferSamples, GL_RGB, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, multiSampledRBO);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Couldn't create multisampled framebuffer correctly.\n";
            return false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Couldn't create regular framebuffer correctly.\n";
            return false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Initialize vertex data.
        unsigned int VBO{ 0 };
        constexpr std::array<float, 24> vertices{
            // pos        // tex
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 1.0f,

            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f,  1.0f, 1.0f, 1.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Initialize kernel matrices.
        constexpr std::array<float, 9> blurKernel{
            1.f / 12.f, 2.f / 12.f, 1.f / 12.f,
            2.f / 12.f, 4.f / 12.f, 1.f / 12.f,
            1.f / 12.f, 2.f / 12.f, 1.f / 12.f,
        };

        constexpr std::array<float, 9> edgeKernel{
            -1.f, -1.f, -1.f,
            -1.f,  8.f, -1.f,
            -1.f, -1.f, -1.f,
        };

        constexpr float offset{ 1.f / 300.f };

        constexpr std::array<std::array<float, 2>, 9> offsets{{
                {{ -offset,  offset }}, // top-left
                {{     0.f,  offset }}, // top-center
                {{  offset,  offset }}, // top-right

                {{ -offset,  0.f    }}, // center-left
                {{     0.f,  0.f    }}, // center-center
                {{  offset,  0.f    }}, // center-right

                {{ -offset, -offset }}, // bottom-left
                {{     0.f, -offset }}, // bottom-center
                {{  offset, -offset }}, // bottom-right
            }};

        // Set shader's uniforms.
        shader->use();
        shader->setUniformArray2Float("offsets", offsets.size(), offsets.data()->data());
        shader->setUniformArrayFloat("edgeKernel", edgeKernel.size(), edgeKernel.data());
        shader->setUniformArrayFloat("blurKernel", blurKernel.size(), blurKernel.data());

        glDeleteBuffers(1, &VBO);

        return true;
    }

    void PostProcessor::beginRender()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, multiSampledFBO);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void PostProcessor::endRender()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, multiSampledFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        // Binds both READ and WRITE framebuffer to default framebuffer!!
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void PostProcessor::render(const float time)
    {
        shader->use();
        shader->setUniformBool("isConfusing", isConfusing);
        shader->setUniformBool("isChaotic", isChaotic);
        shader->setUniformBool("isShaking", isShaking);
        shader->setUniformFloat("time", time);

        glActiveTexture(GL_TEXTURE0);
        texture.bind();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

};
