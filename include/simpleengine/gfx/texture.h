#pragma once

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <gl/glew.h>
#include <gl/gl.h>
#endif

#include <GLFW/glfw3.h>

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
        /**
         * @brief The type of the texture
         * 
         */
        enum Type {
            TexT_DIFFUSE,
            TexT_SPECULAR
        };

        int height;
        int width;
        int channels;
        Type type;

        /**
         * @brief Construct a new Texture object from a path.
         * 
         * @param path The path of the image for the Texture.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(const char* path, Type type = Type::TexT_DIFFUSE, bool img_2d = true, bool mipmap = true);

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param buffer_length The length of the buffer.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(const unsigned char *const buffer, int buffer_length, Type type = Type::TexT_DIFFUSE, bool img_2d = true, bool mipmap = true);

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(std::vector<unsigned char> buffer, Type type = Type::TexT_DIFFUSE, bool img_2d = true, bool mipmap = true);

        static Texture white_texture();

        void bind() const;
        void unbind() const;

        unsigned int get_texture_id() const;
    };
}