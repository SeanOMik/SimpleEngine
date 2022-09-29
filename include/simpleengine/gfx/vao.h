#pragma once

#include <cstdio>
#include <iostream>
#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <gl/glew.h>
#include <gl/gl.h>
#endif

#include "vbo.h"

namespace simpleengine::gfx {
    class VAO {
    public:
        GLuint handle;

        VAO(GLuint handle) : handle(handle) {
            //glCreateVertexArrays(1, &handle);
        }

        ~VAO() {
            std::cout << "~vao(" << handle << ")" << std::endl;
        }

        VAO& operator=(const VAO& other) {
            if (this != &other) {
                handle = other.handle;
            }


            std::cout << "Copied " << handle << std::endl;

            return *this;
        }

        static VAO init() {
            GLuint handle;
            glCreateVertexArrays(1, &handle);

            return VAO(handle);
        }

        void destroy() {
            glDeleteVertexArrays(1, &handle);
        }

        void bind() const {
            glBindVertexArray(handle);
        }

        void unbind() const {
            glBindVertexArray(0);
        }

        // TODO: Fix this.
        void enable_attrib(const VBO& vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset, bool should_bind = true) const {
            if (should_bind) {
                bind();
                vbo.bind();
            }

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
                    glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void *) offset);
                    break;
            }
            glEnableVertexAttribArray(index);

            if (should_bind) {
                // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's
                // bound vertex buffer object so afterwards we can safely unbind.
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this
                // rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally
                // don't unbind VAOs (nor VBOs) when it's not directly necessary.
                glBindVertexArray(0);
            }
        }

        void disable_attrib(const VBO& vbo, GLuint index) const {
            bind();
            vbo.bind();

            glDisableVertexAttribArray(index);
        }

        void set_attrib_value(const VBO& vbo, GLuint index, float f) const {
            bind();
            vbo.bind();

            glVertexAttrib1f(index, f);
        }
    };
}