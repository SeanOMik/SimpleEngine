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
        std::vector<glm::vec3> vertices;
        gfx::VBO vbo;
        gfx::VAO vao;

        Triangle(std::shared_ptr<GLuint> shader_program, std::vector<glm::vec3> vertices) : super(nullptr),
                shader_program(shader_program), vertices(vertices), vbo(gfx::VBO(GL_ARRAY_BUFFER, false)), 
                vao(gfx::VAO()) {

            vao.bind();
            vbo.buffer(vertices.data(), 0, vertices.size() * sizeof(float) * 3); // 3 floats are in each "row" of the vector.
            vao.enable_attrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
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