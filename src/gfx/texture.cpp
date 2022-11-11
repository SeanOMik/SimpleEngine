#include "gfx/texture.h"
#include "log/logger.h"

#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace simpleengine::gfx {
    Texture::Texture(const char* path, aiTextureType type, int flags): type(type), path(path) {
        bool img_2d = flags & TextureFlags::TexFlags_IMG_2D;
        bool flip_vertically = flags & TextureFlags::TexFlags_FLIP_VERTICALLY;
        bool mipmap = flags & TextureFlags::TexFlags_MIPMAP;

        image_type_gl = img_2d ? GL_TEXTURE_2D : GL_TEXTURE_3D;

        glGenTextures(1, &texture_id);
        bind();

        int linear_param = mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

        glTexParameteri(image_type_gl, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(image_type_gl, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(image_type_gl, GL_TEXTURE_MIN_FILTER, linear_param);
        glTexParameteri(image_type_gl, GL_TEXTURE_MAG_FILTER, linear_param);

        stbi_set_flip_vertically_on_load(flip_vertically);

        unsigned char*img_data = stbi_load(path, &width, &height, &channels, 0);
        if(!img_data) {
            const char* failure = stbi_failure_reason();

            SE_CERROR("Failed to load texture! Reason: {}", failure);
            throw std::runtime_error("Failed to load texture from memory!");
        }
        SE_CDEBUG("Loading image! Width = {}px, height = {}px, channel count = {}", width, height, channels);

        // Get the color type
        int color_format = 0;
        if (channels == 1) {
            color_format = GL_RED;
        } else if (channels == 3) {
            color_format = GL_RGB;
        } else if (channels == 4) {
            color_format = GL_RGBA;
        } else {
            SE_CERROR("Unknown texture color format with {} channels!", channels);
            throw std::runtime_error("Unknown texture color format!");
        }

        glTexImage2D(image_type_gl, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, img_data);
        
        if (mipmap) {
            glGenerateMipmap(image_type_gl);
        }

        stbi_set_flip_vertically_on_load(false);
        stbi_image_free(img_data);

        unbind();
    }

    Texture::Texture(const unsigned char *const buffer, int buffer_length, aiTextureType type, int flags): type(type) {
        bool img_2d = flags & TextureFlags::TexFlags_IMG_2D;
        bool flip_vertically = flags & TextureFlags::TexFlags_FLIP_VERTICALLY;
        bool mipmap = flags & TextureFlags::TexFlags_MIPMAP;

        image_type_gl = img_2d ? GL_TEXTURE_2D : GL_TEXTURE_3D;

        glGenTextures(1, &texture_id);
        bind();

        int linear_param = mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

        glTexParameteri(image_type_gl, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(image_type_gl, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(image_type_gl, GL_TEXTURE_MIN_FILTER, linear_param);
        glTexParameteri(image_type_gl, GL_TEXTURE_MAG_FILTER, linear_param);

        stbi_set_flip_vertically_on_load(flip_vertically);

        unsigned char* img_data = stbi_load_from_memory(buffer, buffer_length, &width, &height, &channels, 0);
        if(!img_data) {
            const char* failure = stbi_failure_reason();
            
            SE_CERROR("Failed to load texture! Reason: ", failure);
            throw std::runtime_error("Failed to load texture from memory!");
        }
        SE_CDEBUG("Loading image! Width = {}px, height = {}px, channel count = {}", width, height, channels);

        // Get the color type
        int color_format = 0;
        if (channels == 1) {
            color_format = GL_RED;
        } else if (channels == 3) {
            color_format = GL_RGB;
        } else if (channels == 4) {
            color_format = GL_RGBA;
        } else {
            SE_CERROR("Unknown texture color format with {} channels!", channels);
            throw std::runtime_error("Unknown texture color format!");
        }

        glTexImage2D(image_type_gl, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, img_data);
        
        if (mipmap) {
            glGenerateMipmap(image_type_gl);
        }

        stbi_set_flip_vertically_on_load(false);
        stbi_image_free(img_data);

        unbind();
    }

    Texture::Texture(std::vector<unsigned char> buffer, aiTextureType type, int flags) :
        Texture(buffer.data(), buffer.size(), type, flags) {
        
    }

    Texture Texture::white_texture() {
        // Create the texture
        int width = 128, height = 128;
        int size = width * height * sizeof(unsigned char) * 4;
        unsigned char* data = (unsigned char*) malloc(size);

        for(int i = 0; i < size; i++) {
            data[i] = 255;
        }

        Texture texture;
        texture.image_type_gl = GL_TEXTURE_2D;
        texture.width = width;
        texture.height = height;
        texture.channels = 4;
        texture.type = aiTextureType::aiTextureType_DIFFUSE;
        
        glGenTextures(1, &texture.texture_id);
        texture.bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        texture.unbind();

        free(data);

        return texture;
    }

    void Texture::bind() const {
        glBindTexture(image_type_gl, texture_id);
    }

    void Texture::unbind() const {
        glBindTexture(image_type_gl, 0);
    }

    unsigned int Texture::get_texture_id() const {
        return texture_id;
    }
}