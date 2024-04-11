#pragma once

#include "pch.hpp"

#include <string>
#include <unordered_map>

namespace Resource {

    // used to create textures
    class Texture final {
    public:
        Texture(const int aWidth,
                const int aHeight,
                const int aInternalFormat,
                const unsigned int aImageFormat,
                const int aWrapS,
                const int aWrapT,
                const int aFilterMin,
                const int aFilterMax,
                unsigned char* data);

        // for da mAp initialization
        Texture() = default;

        inline void bind() const { glBindTexture(GL_TEXTURE_2D, id); }

        inline int getWidth() const { return width; }

        inline int getHeight() const { return height; }

        // TODO fix this
        // this function exists for a fucking case that wasn't considered
        void generate(const int aWidth,
                      const int aHeight,
                      const int aInternalFormat,
                      const unsigned int aImageFormat,
                      const int aWrapS,
                      const int aWrapT,
                      const int aFilterMin,
                      const int aFilterMax,
                      unsigned char* data);

        // yep
        unsigned int id;

    private:
        int width;
        int height;
        int internalFormat;
        unsigned int imageFormat;
        int wrapS;
        int wrapT;
        int filterMin;
        int filterMax;
    };

    // used to draw things, once the texture is created, drawable
    // things should be stored in this class to save memory
    // pretty fucking sure this will turn out not being good
    class DrawableTexture final {
    public:
        inline void bind() const { glBindTexture(GL_TEXTURE_2D, id); }

        unsigned int id;
    };

    class TextureManager final {
    public:
        TextureManager();
        ~TextureManager();

        inline Texture* getTexture(const std::string& ID)
        {
            return &textures.at(ID);
        }

    private:
        std::unordered_map<std::string, Texture> textures;
    };

};
