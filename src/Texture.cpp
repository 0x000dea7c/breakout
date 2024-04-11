#include "Texture.hpp"

#include <stb_image.h>

namespace Resource {

    Texture::Texture(const int aWidth,
                     const int aHeight,
                     const int aInternalFormat,
                     const unsigned int aImageFormat,
                     const int aWrapS,
                     const int aWrapT,
                     const int aFilterMin,
                     const int aFilterMax,
                     unsigned char* data)
    {
        generate(aWidth,
                 aHeight,
                 aInternalFormat,
                 aImageFormat,
                 aWrapS,
                 aWrapT,
                 aFilterMin,
                 aFilterMax,
                 data);
    }

    void Texture::generate(const int aWidth,
                           const int aHeight,
                           const int aInternalFormat,
                           const unsigned int aImageFormat,
                           const int aWrapS,
                           const int aWrapT,
                           const int aFilterMin,
                           const int aFilterMax,
                           unsigned char* data)
    {
        glGenTextures(1, &id);

        width = aWidth;
        height = aHeight;
        internalFormat = aInternalFormat;
        imageFormat = aImageFormat;
        wrapS = aWrapS;
        wrapT = aWrapT;
        filterMin = aFilterMin;
        filterMax = aFilterMax;

        bind();

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     internalFormat,
                     width,
                     height,
                     0,
                     imageFormat,
                     GL_UNSIGNED_BYTE,
                     data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    TextureManager::TextureManager()
    {
        // load every god damn texture, cant be bothered now
        stbi_set_flip_vertically_on_load(true);

        // menu
        int width{ 0 }, height{ 0 }, channels{ 0 };
        unsigned char* data{ stbi_load("./res/textures/BIATCHv2.png",
                                       &width,
                                       &height,
                                       &channels,
                                       0) };
        if(!data) {
            std::cerr << "Couldn't load breakout menu\n";
            exit(EXIT_FAILURE);
        }

        stbi_set_flip_vertically_on_load(false);

        textures["MenuTexture"] = Texture(width,
                                          height,
                                          GL_RGBA,
                                          GL_RGBA,
                                          GL_REPEAT,
                                          GL_REPEAT,
                                          GL_LINEAR,
                                          GL_LINEAR,
                                          data);

        stbi_image_free(data);

        // player paddle
        data = stbi_load("./res/textures/paddle.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load paddle.png\n";
            exit(EXIT_FAILURE);
        }

        textures["PaddleTexture"] = Texture(width,
                                            height,
                                            GL_RGBA,
                                            GL_RGBA,
                                            GL_REPEAT,
                                            GL_REPEAT,
                                            GL_LINEAR,
                                            GL_LINEAR,
                                            data);

        stbi_image_free(data);

        // ball
        data = stbi_load("./res/textures/ball.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load ball.png\n";
            exit(EXIT_FAILURE);
        }

        textures["BallTexture"] = Texture(width,
                                          height,
                                          GL_RGBA,
                                          GL_RGBA,
                                          GL_REPEAT,
                                          GL_REPEAT,
                                          GL_LINEAR,
                                          GL_LINEAR,
                                          data);
        stbi_image_free(data);

        // background
        data = stbi_load("./res/textures/background.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load background.png\n";
            exit(EXIT_FAILURE);
        }

        textures["BackgroundTexture"] = Texture(width,
                                                height,
                                                GL_RGBA,
                                                GL_RGBA,
                                                GL_REPEAT,
                                                GL_REPEAT,
                                                GL_LINEAR,
                                                GL_LINEAR,
                                                data);
        stbi_image_free(data);

        // bricks
        data = stbi_load("./res/textures/block.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load block.png\n";
            exit(EXIT_FAILURE);
        }

        textures["BlockTexture"] = Texture(width,
                                           height,
                                           GL_RGB,
                                           GL_RGB,
                                           GL_REPEAT,
                                           GL_REPEAT,
                                           GL_LINEAR,
                                           GL_LINEAR,
                                           data);
        stbi_image_free(data);

        // solid block
        data = stbi_load("./res/textures/blockSolid.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load blockSolid.png\n";
            exit(EXIT_FAILURE);
        }

        textures["BlockSolidTexture"] = Texture(width,
                                                height,
                                                GL_RGB,
                                                GL_RGB,
                                                GL_REPEAT,
                                                GL_REPEAT,
                                                GL_LINEAR,
                                                GL_LINEAR,
                                                data);
        stbi_image_free(data);

        // particle
        data = stbi_load("./res/textures/particle.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load particle.png\n";
            exit(EXIT_FAILURE);
        }

        textures["ParticleTexture"] = Texture(width,
                                              height,
                                              GL_RGBA,
                                              GL_RGBA,
                                              GL_REPEAT,
                                              GL_REPEAT,
                                              GL_LINEAR,
                                              GL_LINEAR,
                                              data);
        stbi_image_free(data);

        // power-up speed
        data = stbi_load("./res/textures/powerUpSpeed.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load powerUpSpeed.png\n";
            exit(EXIT_FAILURE);
        }

        textures["PowerUpSpeed"] = Texture(width,
                                           height,
                                           GL_RGBA,
                                           GL_RGBA,
                                           GL_REPEAT,
                                           GL_REPEAT,
                                           GL_LINEAR,
                                           GL_LINEAR,
                                           data);
        stbi_image_free(data);

        // power-up sticky
        data = stbi_load("./res/textures/powerUpSticky.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load powerUpSticky.png\n";
            exit(EXIT_FAILURE);
        }

        textures["PowerUpSticky"] = Texture(width,
                                            height,
                                            GL_RGBA,
                                            GL_RGBA,
                                            GL_REPEAT,
                                            GL_REPEAT,
                                            GL_LINEAR,
                                            GL_LINEAR,
                                            data);
        stbi_image_free(data);

        // power-up pass through
        data = stbi_load("./res/textures/powerUpPassThrough.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load powerUpPassThrough.png\n";
            exit(EXIT_FAILURE);
        }

        textures["PowerUpPassThrough"] = Texture(width,
                                                 height,
                                                 GL_RGBA,
                                                 GL_RGBA,
                                                 GL_REPEAT,
                                                 GL_REPEAT,
                                                 GL_LINEAR,
                                                 GL_LINEAR,
                                                 data);
        stbi_image_free(data);

        // power-up pad size increase
        data = stbi_load("./res/textures/powerUpPadSizeIncrease.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load powerUpPadSizeIncrease.png\n";
            exit(EXIT_FAILURE);
        }

        textures["PowerUpPadSizeIncrease"] = Texture(width,
                                                     height,
                                                     GL_RGBA,
                                                     GL_RGBA,
                                                     GL_REPEAT,
                                                     GL_REPEAT,
                                                     GL_LINEAR,
                                                     GL_LINEAR,
                                                     data);
        stbi_image_free(data);

        // power-up confuse
        data = stbi_load("./res/textures/powerUpConfuse.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load powerUpConfuse.png\n";
            exit(EXIT_FAILURE);
        }

        textures["PowerUpConfuse"] = Texture(width,
                                             height,
                                             GL_RGBA,
                                             GL_RGBA,
                                             GL_REPEAT,
                                             GL_REPEAT,
                                             GL_LINEAR,
                                             GL_LINEAR,
                                             data);
        stbi_image_free(data);

        // power-up chaos
        data = stbi_load("./res/textures/powerUpChaos.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load powerUpChaos.png\n";
            exit(EXIT_FAILURE);
        }

        textures["PowerUpChaos"] = Texture(width,
                                           height,
                                           GL_RGBA,
                                           GL_RGBA,
                                           GL_REPEAT,
                                           GL_REPEAT,
                                           GL_LINEAR,
                                           GL_LINEAR,
                                           data);
        stbi_image_free(data);

        // heart
        data = stbi_load("./res/textures/heart.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load heart.png\n";
            exit(EXIT_FAILURE);
        }

        textures["Heart"] = Texture(width,
                                    height,
                                    GL_RGBA,
                                    GL_RGBA,
                                    GL_REPEAT,
                                    GL_REPEAT,
                                    GL_LINEAR,
                                    GL_LINEAR,
                                    data);
        stbi_image_free(data);

        // level one
        data = stbi_load("./res/textures/levelOne.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load levelOne.png\n";
            exit(EXIT_FAILURE);
        }

        textures["LevelOne"] = Texture(width,
                                       height,
                                       GL_RGBA,
                                       GL_RGBA,
                                       GL_REPEAT,
                                       GL_REPEAT,
                                       GL_LINEAR,
                                       GL_LINEAR,
                                       data);
        stbi_image_free(data);

        // level two
        data = stbi_load("./res/textures/levelTwo.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load levelTwo.png\n";
            exit(EXIT_FAILURE);
        }

        textures["LevelTwo"] = Texture(width,
                                       height,
                                       GL_RGBA,
                                       GL_RGBA,
                                       GL_REPEAT,
                                       GL_REPEAT,
                                       GL_LINEAR,
                                       GL_LINEAR,
                                       data);
        stbi_image_free(data);

        // level three
        data = stbi_load("./res/textures/levelThree.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load levelThree.png\n";
            exit(EXIT_FAILURE);
        }

        textures["LevelThree"] = Texture(width,
                                         height,
                                         GL_RGBA,
                                         GL_RGBA,
                                         GL_REPEAT,
                                         GL_REPEAT,
                                         GL_LINEAR,
                                         GL_LINEAR,
                                         data);
        stbi_image_free(data);

        // level four
        data = stbi_load("./res/textures/levelFour.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load levelFour.png\n";
            exit(EXIT_FAILURE);
        }

        textures["LevelFour"] = Texture(width,
                                        height,
                                        GL_RGBA,
                                        GL_RGBA,
                                        GL_REPEAT,
                                        GL_REPEAT,
                                        GL_LINEAR,
                                        GL_LINEAR,
                                        data);
        stbi_image_free(data);

        // ground, used for ui
        data = stbi_load("./res/textures/ground.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load ground.png\n";
            exit(EXIT_FAILURE);
        }

        textures["Ground"] = Texture(width,
                                     height,
                                     GL_RGBA,
                                     GL_RGBA,
                                     GL_REPEAT,
                                     GL_REPEAT,
                                     GL_LINEAR,
                                     GL_LINEAR,
                                     data);
        stbi_image_free(data);

        // win and lose bgs
        stbi_set_flip_vertically_on_load(true);
        data = stbi_load("./res/textures/GameOvah.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load GameOvah.png\n";
            exit(EXIT_FAILURE);
        }

        textures["LoseBackground"] = Texture(width,
                                             height,
                                             GL_RGBA,
                                             GL_RGBA,
                                             GL_REPEAT,
                                             GL_REPEAT,
                                             GL_LINEAR,
                                             GL_LINEAR,
                                             data);
        stbi_image_free(data);

        data = stbi_load("./res/textures/GZBro.png",
                         &width,
                         &height,
                         &channels,
                         0);
        if(!data) {
            std::cerr << "Couldn't load GZBro.png\n";
            exit(EXIT_FAILURE);
        }

        textures["WinBackground"] = Texture(width,
                                            height,
                                            GL_RGBA,
                                            GL_RGBA,
                                            GL_REPEAT,
                                            GL_REPEAT,
                                            GL_LINEAR,
                                            GL_LINEAR,
                                            data);
        stbi_image_free(data);
        stbi_set_flip_vertically_on_load(false);
    }

    TextureManager::~TextureManager()
    {
        for(auto& texture: textures) {
            glDeleteTextures(1, &texture.second.id);
        }
    }
};
