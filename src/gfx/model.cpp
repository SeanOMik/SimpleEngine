#include "gfx/model.h"

namespace simpleengine::gfx {
    Model::Model(GLFWwindow* window, gfx::Shader shader, std::vector<Vertex> vertices, std::vector<GLuint> indicies) :
            simpleengine::Renderable(window), shader(shader), vertices(vertices), indicies(indicies), vbo(gfx::VBO(GL_ARRAY_BUFFER, false)),
            ebo(gfx::VBO(GL_ELEMENT_ARRAY_BUFFER, false)) {

        setup_vertexes();
    }

    Model::Model(GLFWwindow* window, std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices, std::vector<GLuint> indicies) :
            Model(window, gfx::Shader(shader_program), vertices, indicies) {

    }

    void Model::setup_vertexes() {
        vao.bind();
        vbo.buffer(vertices.data(), 0, sizeof(Vertex) * vertices.size());
        if (!indicies.empty()) {
            ebo.buffer(indicies.data(), 0, indicies.size() * sizeof(GLuint));
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coord));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Model::update(const float& delta_time) {
        //this->rotate_y(1.f);
    }

    void Model::render(GLFWwindow* target) {
        shader.use();
        shader.set_uniform_matrix_4f("transform_matrix", transform_matrix, false);

        // When binding to the texture, also tell the shader if the texture is set or not.
        shader.set_uniform_int("texture_is_set", (GLint) false, false);
        
        vao.bind();
        if (indicies.empty()) {
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        } else {
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
        }
    }
}