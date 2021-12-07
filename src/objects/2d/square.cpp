#include "objects/2d/shapes/square.h"

namespace simpleengine::objects_2d::shapes {
    Square::Square(gfx::Shader shader, std::vector<Vertex> vertices, std::vector<GLuint> indicies) :
            simpleengine::Renderable(nullptr), shader(shader), vertices(vertices), indicies(indicies),
            ebo(gfx::VBO(GL_ELEMENT_ARRAY_BUFFER, false)), vbo(gfx::VBO(GL_ARRAY_BUFFER, false)),
            texture(nonstd::nullopt) {

        vao.bind();
        vbo.buffer(vertices.data(), 0, sizeof(Vertex) * vertices.size());
        ebo.buffer(indicies.data(), 0, indicies.size() * sizeof(GLuint));

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);
        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coord));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Square::Square(std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices, std::vector<GLuint> indicies) :
        Square(gfx::Shader(shader_program), vertices, indicies) {

    }

    void Square::set_texture(gfx::Texture texture) {
        this->texture = texture;
    }
    
    void Square::update(const float& delta_time) {
        this->rotate_y(1.f);
    }

    void Square::render(GLFWwindow* target) {
        shader.use();

        shader.set_uniform_matrix_4f("transform_matrix", transform_matrix, false);

        // When binding to the texture, also tell the shader if the texture is set or not.
        if (texture.has_value()) {
            shader.set_uniform_int("texture_is_set", true, false);
            texture.value().bind();
        } else {
            shader.set_uniform_int("texture_is_set", false, false);
        }

        vao.bind();
        glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
    }
}