#pragma once

#include "pch.hpp"

#include <filesystem>
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

namespace Resource {

    class Shader final {
    public:
        Shader(const std::filesystem::path& vertPath,
               const std::filesystem::path& fragPath);

        Shader(const std::filesystem::path& vertPath,
               const std::filesystem::path& fragPath,
               const std::filesystem::path& geomPath);

        // for da map initialization
        Shader() = default;

        void use() const;

        void setUniformFloat(const std::string& name,
                             const float value) const;

        void setUniformArrayFloat(const std::string&name,
                                  const unsigned int size,
                                  const float* data) const;

        void setUniformArray2Float(const std::string&name,
                                   const unsigned int size,
                                   const float* data) const;

        void setUniformInt(const std::string& name,
                           const int value) const;

        void setUniformBool(const std::string& name,
                            const bool value) const;

        void setUniformVec2(const std::string& name,
                            const glm::vec2& value) const;

        void setUniformVec3(const std::string& name,
                            const glm::vec3& value) const;

        void setUniformVec4(const std::string& name,
                            const glm::vec4& value) const;

        void setUniformMat4(const std::string& name,
                            const glm::mat4& value) const;

        inline unsigned int getId() const { return id; }

        bool generate(const std::filesystem::path& vertPath,
                      const std::filesystem::path& fragPath);

        bool generate(const std::filesystem::path& vertPath,
                      const std::filesystem::path& fragPath,
                      const std::filesystem::path& geomPath);

    private:
        enum class ShaderType {
            VERTEX,
            FRAGMENT,
            GEOMETRY,
            PROGRAM,
        };

        bool hasCompileErrors(const unsigned int object,
                              const ShaderType type) const;

        unsigned int id;
    };

    // load and compile every game sheydah
    class ShaderManager final {
    public:
        ShaderManager(const float sceneWidth,
                      const float sceneHeight);
        ~ShaderManager();

        inline const Shader* getShader(const std::string& ID) const
        {
            return &shaders.at(ID);
        }

    private:
        std::unordered_map<std::string, Shader> shaders;
    };
};
