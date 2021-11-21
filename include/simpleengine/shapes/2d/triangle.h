//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_TRIANGLE_H
#define SIMPLEENGINE_TRIANGLE_H

#include <gl/glew.h>
#include <gl/gl.h>

#include <GLFW/glfw3.h>

#include "../../renderable.h"

namespace simpleengine::shapes_2d {
    class Triangle : public simpleengine::Renderable {
    private:
        using super = simpleengine::Renderable;
    private:
        std::shared_ptr<GLuint> shader_program;
    public:
        float vertices[9];
        uint32_t vbo;
        uint32_t vao;

        Triangle(std::shared_ptr<GLuint> shader_program) : super(nullptr), shader_program(shader_program), vertices{
                -0.5f, -0.5f, 0.0f, // left  
                0.5f, -0.5f, 0.0f, // right 
                0.0f,  0.5f, 0.0f  // top   
            } {
            
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0); 

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0);
        }

        virtual ~Triangle() = default;
        
        virtual void update(const float& delta_time) override {

        }

        virtual void render(std::shared_ptr<GLFWwindow> target) override {
            glUseProgram(*shader_program);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    };
}

#endif //SIMPLEENGINE_TRIANGLE_H