#pragma once

#ifdef __linux__
#include <GL/glew.h>
#include <GL/gl.h>
#elif
#include <gl/glew.h>
#include <gl/gl.h>
#endif

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../../renderable.h"
#include "../../vertex.h"
#include "../../transformable.h"
#include "../../optional.h"

#include "../../gfx/vbo.h"
#include "../../gfx/vao.h"
#include "../../gfx/texture.h"
#include "../../gfx/shader.h"

#include <stdint.h>
#include <vector>

namespace simpleengine::shapes_2d {
    class Square : public simpleengine::Renderable, public simpleengine::Transformable {
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

        virtual void render(GLFWwindow* target) override;
    };
}