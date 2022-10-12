#pragma once

#include <cstdint>
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
    enum TextureFlags : uint8_t {
        TexFlags_IMG_2D = 0b00000001,
        TexFlags_FLIP_VERTICALLY = 0b00000010,
        TexFlags_FLIP_HORIZONTALLY = 0b00000100,
        TexFlags_MIPMAP = 0b00001000,
        /* TexFlags_NO_COLOR = 0b00010000,
        TexFlags_RGB = 0b00100000,
        TexFlags_RGBA = 0b01000000, */
    };

    class Texture {
    private:
        //unsigned char* img_data; // TODO Free this if its not used anymore
        unsigned int texture_id;

        unsigned int image_type_gl;

        Texture() = default;
    public:
        /**
         * @brief The default Texture flags including the color.
         *
         * The default flags are `TexFlags_IMG_2D | TexFlags_MIPMAP`
         *
         * @see simpleengine::gfx::Texture::default_flags_no_color
         * 
         */
        static constexpr int default_flags = TexFlags_IMG_2D | TexFlags_MIPMAP;

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
        Texture(const char* path, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE, int flags = Texture::default_flags);

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param buffer_length The length of the buffer.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(const unsigned char *const buffer, int buffer_length, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE, int flags = Texture::default_flags);

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(std::vector<unsigned char> buffer, aiTextureType type = aiTextureType::aiTextureType_DIFFUSE, int flags = Texture::default_flags);

        static Texture white_texture();

        void bind() const;
        void unbind() const;

        unsigned int get_texture_id() const;
    };
}