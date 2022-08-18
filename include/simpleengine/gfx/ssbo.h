#pragma once

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <gl/glew.h>
#include <gl/gl.h>
#endif

#include "shader.h"

#include <string>

namespace simpleengine::gfx {
    /**
     * @brief Shader Storage buffer Object
     * 
     * https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
     */
    class SSBO {
    private:
        GLuint bind_index;

        void gen_buffer() {
            glGenBuffers(1, &handle);
        }
    public:
        GLuint handle;
        gfx::Shader& shader;

        SSBO(gfx::Shader& shader, GLuint bind_index) : bind_index(bind_index), shader(shader) {
            gen_buffer();
        }

        SSBO(gfx::Shader& shader, const std::string& ssbo_name) : shader(shader) {
            bind_index = glGetProgramResourceIndex(shader.program, GL_SHADER_STORAGE_BLOCK, ssbo_name.c_str());

            gen_buffer();
        }

        void bind_buffer() {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
        }

        void bind_ssbo() {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind_index, handle);
        }

        void buffer(void* data, size_t offset, size_t size) {
            bind_buffer();

            glBufferData(GL_SHADER_STORAGE_BUFFER, size - offset, data, GL_DYNAMIC_COPY);
        }

        void unbind() {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        }
    };
}