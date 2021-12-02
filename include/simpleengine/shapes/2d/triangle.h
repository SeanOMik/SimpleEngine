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
#include "../../gfx/vbo.h"
#include "../../gfx/vao.h"
#include "../../gfx/shader.h"
#include "../../gfx/texture.h"
#include "../../optional.h"
#include "../../translatable.h"

#include <vector>

namespace simpleengine::shapes_2d {
    class Triangle : public simpleengine::Renderable {
    private:
        using super = simpleengine::Renderable;
    private:
        gfx::Shader shader; // This only stores the shader program
        nonstd::optional<gfx::Texture> texture;
        glm::mat4 translation;
    public:
        std::vector<Vertex> vertices;
        gfx::VBO vbo;
        gfx::VAO vao;

        Triangle(gfx::Shader shader, std::vector<Vertex> vertices);

        Triangle(std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices);

        virtual ~Triangle() = default;

        void set_texture(gfx::Texture texture);

        virtual void update(const float& delta_time) override;
        virtual void render(GLFWwindow* target) override;
    };
}