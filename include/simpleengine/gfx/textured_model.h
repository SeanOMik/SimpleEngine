#pragma once

/* #include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "../vertex.h"
#include "../renderable.h"
#include "../transformable.h" */

#include "ssbo.h"
#include "shader.h"
#include "model.h"
#include "texture.h"

#include <GLFW/glfw3.h>

#include <vector>

namespace simpleengine::gfx {
    class TexturedModel : public simpleengine::gfx::Model {
    public:
        std::vector<gfx::Texture> textures;

        TexturedModel(GLFWwindow* window, gfx::Shader shader, gfx::Texture texture, std::vector<Vertex> vertices,
            std::vector<GLuint> indicies = std::vector<GLuint>());
        TexturedModel(GLFWwindow* window, GLuint shader_program, gfx::Texture texture, std::vector<Vertex> vertices,
            std::vector<GLuint> indicies = std::vector<GLuint>());

        TexturedModel(GLFWwindow* window, gfx::Shader shader, std::vector<gfx::Texture> textures, std::vector<Vertex> vertices,
            std::vector<GLuint> indicies = std::vector<GLuint>());
        TexturedModel(GLFWwindow* window, GLuint shader_program, std::vector<gfx::Texture> textures, std::vector<Vertex> vertices,
            std::vector<GLuint> indicies = std::vector<GLuint>());

        virtual void update(const float& delta_time) override;
        virtual void render(GLFWwindow* target) override;
    };
}