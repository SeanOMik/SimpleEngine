#include "gfx/model.h"

namespace simpleengine::gfx {
    Model::Model(GLFWwindow* window, gfx::Shader shader, std::vector<Vertex> vertices, std::vector<GLuint> indicies) :
            simpleengine::Renderable(window), shader(shader), vertices(vertices), indicies(indicies), vbo(gfx::VBO(GL_ARRAY_BUFFER, false)),
            ebo(gfx::VBO(GL_ELEMENT_ARRAY_BUFFER, false)) {

        setup_vertices();
    }

    Model::Model(GLFWwindow* window, GLuint shader_program, std::vector<Vertex> vertices, std::vector<GLuint> indicies) :
            Model(window, gfx::Shader(shader_program), vertices, indicies) {

    }

    void Model::setup_vertices() {
        vao.bind();
        vbo.buffer(vertices.data(), 0, sizeof(Vertex) * vertices.size());
        if (!indicies.empty()) {
            ebo.buffer(indicies.data(), 0, indicies.size() * sizeof(GLuint));
        }

        // Enable VAO attributes
        vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
        vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
        // Attribute 2 is used for normals
        vao.enable_attrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tex_coord));
        vao.enable_attrib(vbo, 4, 1, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, texture_id));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Model::update(const float& delta_time) {
        this->rotate_y(1.f);
    }

    void Model::render(GLFWwindow* target) {
        shader.use();
        shader.set_uniform_matrix_4f("transform_matrix", transform_matrix, false);

        // When binding to the texture, also tell the shader if the texture is set or not.
        //shader.set_uniform_int("texture_is_set", (GLint) false, false);
        
        vao.bind();
        if (indicies.empty()) {
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        } else {
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
        }
    }
}