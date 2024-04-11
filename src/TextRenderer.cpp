#include "TextRenderer.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

namespace Renderer {
    using namespace Resource;

    TextRenderer::TextRenderer(const std::filesystem::path& fontPath,
                               const unsigned int fontSize,
                               const Shader* aShader)
        :shader{ aShader },
         VAO{ 0 },
         VBO{ 0 }
    {
        // TODO move this to shader manager
        // if(!shader.generate("./shaders/textRenderer.vert",
        //                     "./shaders/textRenderer.frag")) {
        //     std::cerr << "Couldn't generate text renderer shaders.\n";
        //     exit(EXIT_FAILURE);
        // }

        // shader.setUniformInt("image", 0);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        if(!loadCharsFromFont(fontPath, fontSize)) {
            std::cerr << "Couldn't load font from path " << fontPath << '\n';
            exit(EXIT_FAILURE); // blehhh, cant be bothered
        }

    }

    TextRenderer::~TextRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    bool TextRenderer::loadCharsFromFont(const std::filesystem::path& fontPath,
                                         const unsigned int fontSize)
    {
        FT_Library ft;

        if(FT_Init_FreeType(&ft)) {
            std::cerr << "Couldn't initialize FreeType.\n";
            return false;
        }

        FT_Face face;
        if(FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            std::cerr << "Couldn't create new face from path " << fontPath << '\n';
            return false;
        }

        // NOTE when specifying 0, it lets compute the width dynamically based on the height
        FT_Set_Pixel_Sizes(face, 0, fontSize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for(unsigned char c{ 0 }; c < 128; ++c) {
            if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "Couldn't load glyph with index " << c << '\n';
                continue;
            }
            unsigned int id;
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RED,
                         static_cast<int>(face->glyph->bitmap.width),
                         static_cast<int>(face->glyph->bitmap.rows),
                         0,
                         GL_RED,
                         GL_UNSIGNED_BYTE,
                         face->glyph->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character ch{
                .size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                .bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                .textureID = id,
                .advance = static_cast<unsigned int>(face->glyph->advance.x),
            };

            characters[c] = ch;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        return true;
    }

    void TextRenderer::renderText(const std::string_view& text,
                                  float x,
                                  const float y,
                                  const float scale,
                                  const glm::vec3& colour)
    {
        shader->use();
        shader->setUniformVec3("textColour", colour);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        for(auto c = text.cbegin(); c != text.cend(); ++c) {
            const auto ch = characters.at(*c);

            const float w{ static_cast<float>(ch.size.x) * scale };
            const float h{ static_cast<float>(ch.size.y) * scale };

            float xPos = x + static_cast<float>(ch.bearing.x) * scale;
            float yPos = y + (static_cast<float>(characters['H'].bearing.y - ch.bearing.y)) * scale;

            // annoying
            const std::array<std::array<float, 4>, 6> vertices{{
                    {{xPos,     yPos + h, 0.f, 1.f}},
                    {{xPos + w, yPos,     1.f, 0.f}},
                    {{xPos,     yPos,     0.f, 0.f}},

                    {{xPos,     yPos + h, 0.f, 1.f}},
                    {{xPos + w, yPos + h, 1.f, 1.f}},
                    {{xPos + w, yPos,     1.f, 0.f}},
                }};

            glBindTexture(GL_TEXTURE_2D, ch.textureID);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            // NOTE: bitshift by 6 (dividing over 1/2^6) to get value in pixels
            // why 6? because the advance value is given in a unit called "1/64th of a pixel.
            x += (ch.advance >> 6) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

};
