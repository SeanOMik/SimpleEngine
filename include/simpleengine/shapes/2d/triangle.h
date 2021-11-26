//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_TRIANGLE_H
#define SIMPLEENGINE_TRIANGLE_H

#include <gl/glew.h>
#include <gl/gl.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../../renderable.h"
#include "../../vertex.h"

#include "../../gfx/vbo.h"
#include "../../gfx/vao.h"
#include "simpleengine/gfx/vao.h"

#include <vector>

namespace simpleengine::shapes_2d {
    class Triangle : public simpleengine::Renderable {
    private:
        using super = simpleengine::Renderable;
    private:
        std::shared_ptr<GLuint> shader_program;
    public:
        std::vector<Vertex> vertices;
        gfx::VBO vbo;
        gfx::VAO vao;

        Triangle(std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices) : super(nullptr),
                shader_program(shader_program), vertices(vertices), vbo(gfx::VBO(GL_ARRAY_BUFFER, false)), 
                vao(gfx::VAO()) {

            vao.bind();
            vbo.buffer(vertices.data(), 0, sizeof(Vertex) * vertices.size());

            /* vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
            vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
            vao.enable_attrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tex_coord)); */

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);
            
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coord));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            /* glCreateVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);
            
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coord));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0); */
        }

        virtual ~Triangle() = default;
        
        virtual void update(const float& delta_time) override {

        }

        virtual void render(std::shared_ptr<GLFWwindow> target) override {
            glUseProgram(*shader_program);

            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }
    };
}

#endif //SIMPLEENGINE_TRIANGLE_H