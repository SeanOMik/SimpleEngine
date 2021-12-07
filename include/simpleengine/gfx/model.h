#pragma once

#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "../vertex.h"
#include "../renderable.h"
#include "../transformable.h"

#include <vector>

namespace simpleengine::gfx {
    class Model : public simpleengine::Renderable, public simpleengine::Transformable {
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indicies;
        gfx::VBO ebo;
        gfx::VBO vbo;
        gfx::VAO vao;

        gfx::Shader shader;

        Model(GLFWwindow* window, gfx::Shader shader, std::vector<Vertex> vertices, std::vector<GLuint> indicies = std::vector<GLuint>());
        Model(GLFWwindow* window, std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices,
            std::vector<GLuint> indicies = std::vector<GLuint>());
    protected:
        void setup_vertexes();
    public:

        virtual void update(const float& delta_time) override;
        virtual void render(GLFWwindow* target) override;
    };
}