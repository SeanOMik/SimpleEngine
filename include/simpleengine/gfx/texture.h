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

namespace simpleengine {
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
        Texture(const char* path, bool img_2d = true, bool mipmap = true) {
            image_type = img_2d ? GL_TEXTURE_2D : GL_TEXTURE_3D;

            glGenTextures(1, &texture_id);
            bind();

            glTexParameteri(image_type, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(image_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(image_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(image_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            img_data = SOIL_load_image
            (
                path,
                &width, &height, &channels,
                SOIL_LOAD_RGBA
            );

            if (!img_data) {
                std::cerr << "Failed to load texture from memory! (" << SOIL_last_result() << ")" << std::endl;
                throw std::runtime_error("Failed to load texture from memory!");
            }

            glTexImage2D(image_type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
            
            if (mipmap) {
                glGenerateMipmap(image_type);
            }

            SOIL_free_image_data(img_data);
            glBindTexture(image_type, 0);
        }

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param buffer_length The length of the buffer.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(const unsigned char *const buffer, int buffer_length, bool img_2d = true, bool mipmap = true) {
            image_type = img_2d ? GL_TEXTURE_2D : GL_TEXTURE_3D;

            glGenTextures(1, &texture_id);
            bind();

            glTexParameteri(image_type, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(image_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(image_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(image_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            img_data = SOIL_load_image_from_memory
            (
                buffer, buffer_length,
                &width, &height, &channels,
                SOIL_LOAD_RGBA
            );

            if (!img_data) {
                std::cerr << "Failed to load texture from memory! (" << SOIL_last_result() << ")" << std::endl;
                throw std::runtime_error("Failed to load texture from memory!");
            }

            glTexImage2D(image_type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
            
            if (mipmap) {
                glGenerateMipmap(image_type);
            }

            SOIL_free_image_data(img_data);
            glBindTexture(image_type, 0);
        }

        /**
         * @brief Construct a new Texture object from the loaded file buffer.
         * 
         * @param buffer The bytes of the loaded file.
         * @param img_2d Whether or not the texture is 2D.
         * @param mipmap Whether or not to generate mipmaps for this texture.
         */
        Texture(std::vector<unsigned char> buffer, bool img_2d = true, bool mipmap = true) :
            Texture(buffer.data(), buffer.size(), img_2d, mipmap) {
            
        }

        /* ~Texture() {
            if (img_data != nullptr) {
                SOIL_free_image_data(img_data);
            }
        } */

        void bind() const {
            glBindTexture(image_type, texture_id);
        }
    };
}

#endif