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
#include "../../gfx/shader.h"
#include "../../gfx/texture.h"
#include "../../optional.h"

#include <vector>

namespace simpleengine::shapes_2d {
    class Triangle : public simpleengine::Renderable {
    private:
        using super = simpleengine::Renderable;
    private:
        gfx::Shader shader; // This only stores the shader program
        nonstd::optional<gfx::Texture> texture;
    public:
        std::vector<Vertex> vertices;
        gfx::VBO vbo;
        gfx::VAO vao;

        Triangle(gfx::Shader shader, std::vector<Vertex> vertices);

        Triangle(std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices);

        virtual ~Triangle() = default;

        void set_texture(gfx::Texture texture);

        virtual void update(const float& delta_time) override;

        virtual void render(std::shared_ptr<GLFWwindow> target) override;
    };
}

#endif //SIMPLEENGINE_TRIANGLE_H