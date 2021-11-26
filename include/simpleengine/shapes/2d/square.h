//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_SQUARE_H
#define SIMPLEENGINE_SQUARE_H

#include <gl/glew.h>
#include <gl/gl.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../../renderable.h"
#include "../../vertex.h"

#include "../../gfx/vbo.h"
#include "../../gfx/vao.h"
#include "simpleengine/gfx/vao.h"

#include <stdint.h>
#include <vector>

namespace simpleengine::shapes_2d {
    class Square : public simpleengine::Renderable {
    private:
        using super = simpleengine::Renderable;
    private:
        std::shared_ptr<GLuint> shader_program;
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indicies;
        gfx::VBO ebo;
        gfx::VBO vbo;
        gfx::VAO vao;

        Square(std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices, std::vector<GLuint> indicies) :
                super(nullptr), shader_program(shader_program), vertices(vertices), indicies(indicies),
                ebo(gfx::VBO(GL_ELEMENT_ARRAY_BUFFER, false)), vbo(gfx::VBO(GL_ARRAY_BUFFER, false)),
                vao(gfx::VAO()) {

            vao.bind();
            vbo.buffer(vertices.data(), 0, sizeof(Vertex) * vertices.size());
            ebo.buffer(indicies.data(), 0, indicies.size() * sizeof(GLuint));

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);
            
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
            glEnableVertexAttribArray(1);
            
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coord));
            glEnableVertexAttribArray(2);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        virtual ~Square() = default;
        
        virtual void update(const float& delta_time) override {

        }

        virtual void render(std::shared_ptr<GLFWwindow> target) override {
            glUseProgram(*shader_program);

            vao.bind();
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
        }
    };
}

#endif //SIMPLEENGINE_TRIANGLE_H