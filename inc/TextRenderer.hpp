#pragma once

#include "pch.hpp"

#include <glm/glm.hpp>

#include "Shader.hpp"

#include <filesystem>
#include <string_view>
#include <string>
#include <unordered_map>

namespace Renderer {

    class TextRenderer final {
    public:
        TextRenderer(const std::filesystem::path& fontPath,
                     const unsigned int fontSize,
                     const Resource::Shader* aShader);

        ~TextRenderer();

        bool loadCharsFromFont(const std::filesystem::path& fontPath,
                               const unsigned int fontSize);

        void renderText(const std::string_view& text,
                        float x,
                        const float y,
                        const float scale,
                        const glm::vec3& colour);

    private:
        struct Character final {
            glm::ivec2 size;
            glm::ivec2 bearing;
            unsigned int textureID;
            unsigned int advance;
        };

        std::unordered_map<unsigned char, Character> characters;
        const Resource::Shader* shader;
        unsigned int VAO;
        unsigned int VBO;
    };

};
