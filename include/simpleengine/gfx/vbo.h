//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_VBO_H
#define SIMPLEENGINE_VBO_H

#include <gl/glew.h>
#include <gl/GL.h>

namespace simpleengine::gfx {
    class VBO {
    public:
        GLuint handle;
        GLint type;
        bool dynamic;

        VBO(GLint type, bool dynamic) : type(type), dynamic(dynamic) {
            glGenBuffers(1, &handle);
        }

        ~VBO() {
            destroy();
        }

        void destroy() {
            glDeleteBuffers(1, &handle);
        }

        void bind() {
            glBindBuffer(type, handle);
        }

        void buffer(void *data, size_t offset, size_t size) {
            bind();
            glBufferData(type, size - offset, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
    };
}

#endif