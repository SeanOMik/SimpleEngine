#include "gfx/texture.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace simpleengine::gfx {
    Texture::Texture(const char* path, aiTextureType type, int flags): type(type), path(path) {
        bool img_2d = flags & TextureFlags::TexFlags_IMG_2D;
        bool flip_vertically = flags & TextureFlags::TexFlags_FLIP_VERTICALLY;
        bool mipmap = flags & TextureFlags::TexFlags_MIPMAP;

        // Get the color channel type for opengl, and get 
        // the channel count for loading the texture with stb_image
        int gl_color_channels;
        int channel_count;
        if (flags & TexFlags_RGBA) {
            channel_count = 4;
            gl_color_channels = GL_RGBA;
        } else if (flags & TexFlags_RGB) {
            channel_count = 3;
            gl_color_channels = GL_RGB;
        } else if (flags & TexFlags_NO_COLOR) {
            channel_count = 1;
            gl_color_channels = GL_RED;
        } else {
            std::cerr << "Texture color flag is missing!! Specify TexFlags_RGBA" << std::endl;
            throw std::runtime_error("Texture color flag is missing!! Specify TexFlags_RGBA");
        }

        image_type_gl = img_2d ? GL_TEXTURE_2D : GL_TEXTURE_3D;

        glGenTextures(1, &texture_id);
        bind();

        int linear_param = mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

        glTexParameteri(image_type_gl, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(image_type_gl, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(image_type_gl, GL_TEXTURE_MIN_FILTER, linear_param);
        glTexParameteri(image_type_gl, GL_TEXTURE_MAG_FILTER, linear_param);

        stbi_set_flip_vertically_on_load(flip_vertically);

        img_data = stbi_load(path, &width, &height, &channels, channel_count);
        if(!img_data) {
            const char* failure = stbi_failure_reason();
            std::cerr << "Failed to load texture! (" << failure << ")" << std::endl;
            throw std::runtime_error("Failed to load texture from memory!");
        }
        std::cout << "Loaded image with a width of " << width << "px, a height of " << height << "px and " << channels << " channels" << std::endl;

        glTexImage2D(image_type_gl, 0, gl_color_channels, width, height, 0, gl_color_channels, GL_UNSIGNED_BYTE, img_data);
        
        if (mipmap) {
            glGenerateMipmap(image_type_gl);
        }

        stbi_set_flip_vertically_on_load(false);

        unbind();
    }

    Texture::Texture(const unsigned char *const buffer, int buffer_length, aiTextureType type, int flags): type(type) {
        bool img_2d = flags & TextureFlags::TexFlags_IMG_2D;
        bool flip_vertically = flags & TextureFlags::TexFlags_FLIP_VERTICALLY;
        bool mipmap = flags & TextureFlags::TexFlags_MIPMAP;

        // Get the color channel type for opengl, and get 
        // the channel count for loading the texture with stb_image
        int gl_color_channels;
        int channel_count;
        if (flags & TexFlags_RGBA) {
            channel_count = 4;
            gl_color_channels = GL_RGBA;
        } else if (flags & TexFlags_RGB) {
            channel_count = 3;
            gl_color_channels = GL_RGB;
        } else if (flags & TexFlags_NO_COLOR) {
            channel_count = 1;
            gl_color_channels = GL_RED;
        } else {
            std::cerr << "Texture color flag is missing!! Specify TexFlags_RGBA" << std::endl;
            throw std::runtime_error("Texture color flag is missing!! Specify TexFlags_RGBA");
        }

        image_type_gl = img_2d ? GL_TEXTURE_2D : GL_TEXTURE_3D;

        glGenTextures(1, &texture_id);
        bind();

        int linear_param = mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;

        glTexParameteri(image_type_gl, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(image_type_gl, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(image_type_gl, GL_TEXTURE_MIN_FILTER, linear_param);
        glTexParameteri(image_type_gl, GL_TEXTURE_MAG_FILTER, linear_param);

        stbi_set_flip_vertically_on_load(flip_vertically);

        img_data = stbi_load_from_memory(buffer, buffer_length, &width, &height, &channels, channel_count);
        if(!img_data) {
            const char* failure = stbi_failure_reason();
            std::cerr << "Failed to load texture! (" << failure << ")" << std::endl;
            throw std::runtime_error("Failed to load texture from memory!");
        }
        std::cout << "Loaded image with a width of " << width << "px, a height of " << height << "px and " << channels << " channels" << std::endl;

        glTexImage2D(image_type_gl, 0, gl_color_channels, width, height, 0, gl_color_channels, GL_UNSIGNED_BYTE, img_data);
        
        if (mipmap) {
            glGenerateMipmap(image_type_gl);
        }

        stbi_set_flip_vertically_on_load(false);

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
        texture.img_data = data;
        
        glGenTextures(1, &texture.texture_id);
        texture.bind();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.img_data);

        texture.unbind();

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