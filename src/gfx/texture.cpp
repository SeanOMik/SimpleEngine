#include "gfx/texture.h"

namespace simpleengine::gfx {
    Texture::Texture(const char* path, bool img_2d, bool mipmap) {
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
    Texture::Texture(const unsigned char *const buffer, int buffer_length, bool img_2d, bool mipmap) {
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
    Texture::Texture(std::vector<unsigned char> buffer, bool img_2d, bool mipmap) :
        Texture(buffer.data(), buffer.size(), img_2d, mipmap) {
        
    }

    void Texture::bind() const {
        glBindTexture(image_type, texture_id);
    }
}