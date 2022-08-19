#pragma once

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

        //VBO() = default;

        VBO(GLuint handle, GLint type, bool dynamic) : type(type), dynamic(dynamic), handle(handle) {
            //glGenBuffers(1, &handle);
        }

        ~VBO() {
            destroy();
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

        void buffer(void *data, size_t offset, size_t size) {
            bind();
            glBufferData(type, size - offset, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
    };
}