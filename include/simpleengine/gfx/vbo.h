#pragma once

#include <iostream>

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <gl/glew.h>
#include <gl/gl.h>
#endif

namespace simpleengine::gfx {
    class VBO {
    public:
        GLuint handle;
        GLint type;
        bool dynamic;

        VBO(GLuint handle, GLint type, bool dynamic) : type(type), dynamic(dynamic), handle(handle) {
            
        }

        ~VBO() {
            std::cout << "~vbo(" << handle << ")" << std::endl;
        }

        static VBO init(GLint type, bool dynamic) {
            GLuint handle;
            glGenBuffers(1, &handle);

            return VBO(handle, type, dynamic);
        }

        void destroy() {
            glDeleteBuffers(1, &handle);
        }

        void bind() const {
            glBindBuffer(type, handle);
        }

        void unbind() const {
            glBindBuffer(type, 0);
        }

        void buffer(void *data, size_t offset, size_t size) {
            bind();
            glBufferData(type, size - offset, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
    };
}