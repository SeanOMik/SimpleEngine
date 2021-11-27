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
#include "../../gfx/texture.h"
#include "../../gfx/shader.h"

#include <stdint.h>
#include <vector>

#include "../../optional.h"

namespace simpleengine::shapes_2d {
    class Square : public simpleengine::Renderable {
    private:
        using super = simpleengine::Renderable;
    private:
        gfx::Shader shader;
        nonstd::optional<gfx::Texture> texture;
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indicies;
        gfx::VBO ebo;
        gfx::VBO vbo;
        gfx::VAO vao;

        Square(gfx::Shader shader, std::vector<Vertex> vertices, std::vector<GLuint> indicies);

        Square(std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices, std::vector<GLuint> indicies);

        virtual ~Square() = default;

        void set_texture(gfx::Texture texture);
        
        virtual void update(const float& delta_time) override;

        virtual void render(std::shared_ptr<GLFWwindow> target) override;
    };
}

#endif //SIMPLEENGINE_TRIANGLE_H