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
        nonstd::optional<Texture> texture;
    public:
        std::vector<Vertex> vertices;
        gfx::VBO vbo;
        gfx::VAO vao;

        Triangle(gfx::Shader shader, std::vector<Vertex> vertices) : super(nullptr),
                shader(shader), vertices(vertices), vbo(gfx::VBO(GL_ARRAY_BUFFER, false)), texture(nonstd::nullopt) {
            vao.bind();
            vbo.buffer(vertices.data(), 0, sizeof(Vertex) * vertices.size());

            vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
            vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
            vao.enable_attrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tex_coord));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        Triangle(std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices) :
                Triangle(gfx::Shader(shader_program), vertices) {

        }

        virtual ~Triangle() = default;

        void set_texture(Texture texture) {
            this->texture = texture;
        }

        virtual void update(const float& delta_time) override {

        }

        virtual void render(std::shared_ptr<GLFWwindow> target) override {
            shader.use();

            // If theres a texture set, tell the fragment shader that and bind to the texture for drawing.
            if (texture.has_value()) {
                shader.setUniformInt("texture_is_set", true, false);
                texture.value().bind();
            } else {
                shader.setUniformInt("texture_is_set", false, false);
            }
            
            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }
    };
}

#endif //SIMPLEENGINE_TRIANGLE_H