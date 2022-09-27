#pragma once

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <gl/glew.h>
#include <gl/gl.h>
#endif

#include <assimp/material.h>

#include <glm/glm.hpp>

#include <stdexcept>
#include <vector>
#include <iostream>

namespace simpleengine::gfx {
    class Texture {
    private:
        unsigned char* img_data; // TODO Free this if its not used anymore
        unsigned int texture_id;

        unsigned int image_type_gl;

        Texture() = default;
    public:
        int height;
        int width;
        int channels;
        aiTextureType type;
        std::string path;

        /**
         * @brief Construct a new Texture object from a path.
         * 
         * @param path The path of the image for the Texture.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(const char* path, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE, bool img_2d = true, bool flip_vertically = false, bool mipmap = true);

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param buffer_length The length of the buffer.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(const unsigned char *const buffer, int buffer_length, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE, bool img_2d = true, bool flip_vertically = false, bool mipmap = true);

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(std::vector<unsigned char> buffer, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE, bool img_2d = true, bool flip_vertically = false, bool mipmap = true);

        static Texture white_texture();

        void bind() const;
        void unbind() const;

        unsigned int get_texture_id() const;
    };
}