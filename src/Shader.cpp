#include "Shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

namespace Resource {

    // TODO terribad
    static bool gShaderError{ false };

    Shader::Shader(const std::filesystem::path& vertPath,
                   const std::filesystem::path& fragPath)
    {
        if(!generate(vertPath, fragPath)) {
            std::cerr << "Couldn't generate vertex and fragment shader.\n";
            gShaderError = true;
        }
    }

    Shader::Shader(const std::filesystem::path& vertPath,
                   const std::filesystem::path& fragPath,
                   const std::filesystem::path& geomPath)
    {
        if(!generate(vertPath, fragPath, geomPath)) {
            std::cerr << "Couldn't generate vertex, fragment and geometry shader.\n";
            gShaderError = true;
        }
    }

    void Shader::use() const
    {
        glUseProgram(id);
    }

    void Shader::setUniformFloat(const std::string& name,
                                 const float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void Shader::setUniformInt(const std::string& name,
                               const int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void Shader::setUniformVec2(const std::string& name,
                                const glm::vec2& value) const
    {
        glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
    }

    void Shader::setUniformVec3(const std::string& name,
                                const glm::vec3& value) const
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setUniformVec4(const std::string& name,
                                const glm::vec4& value) const
    {
        glUniform4f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void Shader::setUniformMat4(const std::string& name,
                                const glm::mat4& value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, false, glm::value_ptr(value));
    }

    void Shader::setUniformBool(const std::string& name,
                                const bool value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<bool>(value));
    }

    bool Shader::hasCompileErrors(const unsigned int object,
                                  const ShaderType type) const
    {
        int success{ 0 };
        std::array<char, 1024> log;

        switch(type) {
        case ShaderType::VERTEX:
        case ShaderType::FRAGMENT:
        case ShaderType::GEOMETRY:
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(object, log.size(), nullptr, &log[0]);
                std::cerr << "Couldn't compile shader type " << static_cast<int>(type) << " : " << log.data() << '\n';
                return true;
            }
            break;
        case ShaderType::PROGRAM:
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(object, log.size(), nullptr, &log[0]);
                std::cerr << "Couldn't link program: " << log.data() << '\n';
                return true;
            }
            break;
        }

        return false;
    }

    void Shader::setUniformArrayFloat(const std::string& name,
                                      const unsigned int size,
                                      const float* data) const
    {
        glUniform1fv(glGetUniformLocation(id, name.c_str()), size, data);
    }

    void Shader::setUniformArray2Float(const std::string&name,
                                       const unsigned int size,
                                       const float* data) const
    {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), size, data);
    }

    bool Shader::generate(const std::filesystem::path& vertPath,
                          const std::filesystem::path& fragPath)
    {
        std::ifstream vertFileifs(vertPath);
        if(!vertFileifs) {
            std::cerr << "Couldn't open vertex file: " << vertPath << '\n';
            return false;
        }

        std::ifstream fragFileifs(fragPath);
        if(!fragFileifs) {
            std::cerr << "Couldn't open fragment file: " << fragPath << '\n';
            return false;
        }

        std::stringstream vertFileSS;
        vertFileSS << vertFileifs.rdbuf();

        std::stringstream fragFileSS;
        fragFileSS << fragFileifs.rdbuf();


        unsigned int vertShader{ 0 }, fragShader{ 0 };
        const std::string vertSrc{ vertFileSS.str() };
        const std::string fragSrc{ fragFileSS.str() };
        const char* vertSrcC{ vertSrc.c_str() };
        const char* fragSrcC{ fragSrc.c_str() };

        vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &vertSrcC, nullptr);
        glCompileShader(vertShader);
        bool hasErrors{ hasCompileErrors(vertShader, ShaderType::VERTEX) };
        if(hasErrors) {
            glDeleteShader(vertShader);
            return false;
        }

        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragSrcC, nullptr);
        glCompileShader(fragShader);
        hasErrors = hasCompileErrors(fragShader, ShaderType::FRAGMENT);
        if(hasErrors) {
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);
            return false;
        }

        id = glCreateProgram();
        glAttachShader(id, vertShader);
        glAttachShader(id, fragShader);

        glLinkProgram(id);
        hasErrors = hasCompileErrors(id, ShaderType::PROGRAM);

        // no need to delete the program, the destructor does that
        if(hasErrors) {
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);
            return false;
        }

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);

        return true;
    }

    bool Shader::generate(const std::filesystem::path& vertPath,
                          const std::filesystem::path& fragPath,
                          const std::filesystem::path& geomPath)
    {
        std::ifstream vertFileifs(vertPath);
        if(!vertFileifs) {
            std::cerr << "Couldn't open vertex file: " << vertPath << '\n';
            return false;
        }

        std::ifstream fragFileifs(fragPath);
        if(!fragFileifs) {
            std::cerr << "Couldn't open fragment file: " << fragPath << '\n';
            return false;
        }

        std::ifstream geomFileifs(geomPath);
        if(!geomFileifs) {
            std::cerr << "Couldn't open geometry file: " << geomPath << '\n';
            return false;
        }

        std::stringstream vertFileSS;
        vertFileSS << vertFileifs.rdbuf();

        std::stringstream fragFileSS;
        fragFileSS << fragFileifs.rdbuf();

        std::stringstream geomFileSS;
        geomFileSS << geomFileifs.rdbuf();

        const std::string vertSrc{ vertFileSS.str().c_str() };
        const std::string fragSrc{ fragFileSS.str().c_str() };
        const std::string geomSrc{ geomFileSS.str().c_str() };
        const char* vertSrcC{ vertSrc.c_str() };
        const char* fragSrcC{ fragSrc.c_str() };
        const char* geomSrcC{ geomSrc.c_str() };

        unsigned int vertShader{ 0 }, fragShader{ 0 }, geomShader{ 0 };

        vertShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertShader, 1, &vertSrcC, nullptr);
        glCompileShader(vertShader);

        bool hasErrors{ hasCompileErrors(vertShader, ShaderType::VERTEX) };
        if(hasErrors) {
            glDeleteShader(vertShader);
            return false;
        }

        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragSrcC, nullptr);
        glCompileShader(fragShader);
        hasErrors = hasCompileErrors(fragShader, ShaderType::FRAGMENT);
        if(hasErrors) {
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);
            return false;
        }

        geomShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geomShader, 1, &geomSrcC, nullptr);
        glCompileShader(geomShader);
        hasErrors = hasCompileErrors(geomShader, ShaderType::GEOMETRY);
        if(hasErrors) {
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);
            glDeleteShader(geomShader);
            return false;
        }

        id = glCreateProgram();
        glAttachShader(id, vertShader);
        glAttachShader(id, fragShader);
        glAttachShader(id, geomShader);

        glLinkProgram(id);
        hasErrors = hasCompileErrors(id, ShaderType::PROGRAM);
        if(hasErrors) {
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);
            glDeleteShader(geomShader);
            return false;
        }

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
        glDeleteShader(geomShader);

        return true;
    }

    ShaderManager::ShaderManager(const float sceneWidth,
                                 const float sceneHeight)
    {
        // this projection matrix is used all over the place
        const glm::mat4 projection{ glm::ortho(0.f,
                                               sceneWidth,
                                               sceneHeight,
                                               0.f,
                                               -1.f,
                                               1.f) };

        // load and compile every damn shader
        shaders["MenuShader"] = Shader("./res/shaders/menu.vert",
                                       "./res/shaders/menu.frag");
        if(gShaderError) {
            std::cerr << "Couldn't compile MenuShader.\n";
            exit(EXIT_FAILURE);
        }

        shaders["MenuShader"].use();
        shaders["MenuShader"].setUniformInt("image", 0);

        shaders["TextRendererShader"] = Shader("./res/shaders/textRenderer.vert",
                                               "./res/shaders/textRenderer.frag");
        if(gShaderError) {
            std::cerr << "Couldn't compile TextRendererShader.\n";
            exit(EXIT_FAILURE);
        }

        // text RendeRARRRR
        shaders["TextRendererShader"].use();
        shaders["TextRendererShader"].setUniformInt("image", 0);
        shaders["TextRendererShader"].setUniformMat4("projection", projection);

        shaders["SpriteShader"] = Shader("./res/shaders/sprite.vert",
                                         "./res/shaders/sprite.frag");
        if(gShaderError) {
            std::cerr << "Couldn't compile SpriteShader.\n";
            exit(EXIT_FAILURE);
        }

        // sprite shader, it's the one all use
        shaders["SpriteShader"].use();
        shaders["SpriteShader"].setUniformInt("image", 0);
        shaders["SpriteShader"].setUniformMat4("projection", projection);

        shaders["ParticleShader"] = Shader("./res/shaders/particles.vert",
                                           "./res/shaders/particles.frag");
        if(gShaderError) {
            std::cerr << "Couldn't compile ParticleShader.\n";
            exit(EXIT_FAILURE);
        }

        shaders["ParticleShader"].use();
        shaders["ParticleShader"].setUniformInt("image", 0);
        shaders["ParticleShader"].setUniformMat4("projection", projection);

        // post processor
        shaders["PostProcessorShader"] = Shader("./res/shaders/postProcessing.vert",
                                                "./res/shaders/postProcessing.frag");
        if(gShaderError) {
            std::cerr << "Couldn't compile PostProcessorShader.\n";
            exit(EXIT_FAILURE);
        }

        shaders["PostProcessorShader"].use();
        shaders["PostProcessorShader"].setUniformInt("image", 0);

        // ui renderer
        shaders["UIRendererShader"] = Shader("./res/shaders/uiRenderer.vert",
                                             "./res/shaders/uiRenderer.frag");
        if(gShaderError) {
            std::cerr << "Couldn't compile UIRendererShader.\n";
            exit(EXIT_FAILURE);
        }

        shaders["UIRendererShader"].use();
        shaders["UIRendererShader"].setUniformInt("image", 0);
        shaders["UIRendererShader"].setUniformMat4("projection", projection);

    }

    ShaderManager::~ShaderManager()
    {
        for(auto& shader : shaders) {
            glDeleteProgram(shader.second.getId());
        }
    }
};
