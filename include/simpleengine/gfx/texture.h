//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_TEXTURE_H
#define SIMPLEENGINE_TEXTURE_H

#include <gl/glew.h>
#include <gl/gl.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <SOIL2/SOIL2.h>

#include <stdexcept>
#include <vector>
#include <iostream>

namespace simpleengine::gfx {
    class Texture {
    private:
        unsigned char* img_data;
        unsigned int texture_id;

        unsigned int image_type;
    public:
        int height;
        int width;
        int channels;

        /**
         * @brief Construct a new Texture object from a path.
         * 
         * @param path The path of the image for the Texture.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(const char* path, bool img_2d = true, bool mipmap = true);

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param buffer_length The length of the buffer.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(const unsigned char *const buffer, int buffer_length, bool img_2d = true, bool mipmap = true);

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(std::vector<unsigned char> buffer, bool img_2d = true, bool mipmap = true);

        void bind() const;
    };
}

#endif