//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_VAO_H
#define SIMPLEENGINE_VAO_H

#include <gl/glew.h>
#include <gl/GL.h>

#include "vbo.h"

namespace simpleengine::gfx {
    class VAO {
    public:
        GLuint handle;

        VAO() {
            glCreateVertexArrays(1, &handle);
        }

        ~VAO() {
            destroy();
        }

        void destroy() {
            glDeleteVertexArrays(1, &handle);
        }

        void bind() {
            glBindVertexArray(handle);
        }

        void attr(VBO vbo) {
            bind();
            vbo.bind();

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0);
        }

        // TODO: Fix this.
        void enable_attrib(VBO vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset) {
            bind();
            vbo.bind();

            // NOTE: glVertexAttribPointer will AUTO-CONVERT integer values to floating point.
            // Integer vertex attributes must be specified with glVertexAttribIPointer.
            // THIS IS EVIL. OpenGL is bad. Who designed this to fail silently?
            switch (type) {
                case GL_BYTE:
                case GL_UNSIGNED_BYTE:
                case GL_SHORT:
                case GL_UNSIGNED_SHORT:
                case GL_INT:
                case GL_UNSIGNED_INT:
                case GL_INT_2_10_10_10_REV:
                case GL_UNSIGNED_INT_2_10_10_10_REV:
                    glVertexAttribIPointer(index, size, type, stride, (void *) offset);
                    break;
                default:
                    glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void *) 0);
                    break;
            }
            glEnableVertexAttribArray(index);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's
            // bound vertex buffer object so afterwards we can safely unbind.
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this
            // rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally
            // don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);
        }
    };
}

#endif