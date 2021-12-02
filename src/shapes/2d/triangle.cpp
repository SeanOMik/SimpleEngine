#include "shapes/2d/triangle.h"

namespace simpleengine::shapes_2d {
    Triangle::Triangle(gfx::Shader shader, std::vector<Vertex> vertices) : super(nullptr),
            shader(shader), vertices(vertices), vbo(gfx::VBO(GL_ARRAY_BUFFER, false)),
            texture(nonstd::nullopt), translation(glm::mat4(1.0f)) {
        vao.bind();
        vbo.buffer(vertices.data(), 0, sizeof(Vertex) * vertices.size());

        vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position));
        vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, color));
        vao.enable_attrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tex_coord));

        // Tell the shader that there is no texture.
        shader.use();
        shader.set_uniform_int("texture_is_set", false, false);

        /* translation = glm::rotate(translation, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        translation = glm::scale(translation, glm::vec3(0.5, 0.5, 0.5)); */ 
        shader.set_uniform_matrix_4f("translation", translation, false);
        shader.unuse();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    Triangle::Triangle(std::shared_ptr<GLuint> shader_program, std::vector<Vertex> vertices) :
            Triangle(gfx::Shader(shader_program), vertices) {

    }

    void Triangle::set_texture(gfx::Texture texture) {
        this->texture = texture;
        
        // Tell the shader that there is a texture set.
        shader.set_uniform_int("texture_is_set", true);
    }

    void Triangle::update(const float& delta_time) {
        /* for (Vertex& vertex : vertices) {
            vertex.position.translate_x(0.01f);
            vertex.position.translate_y(0.01f);
        } */
    }

    void Triangle::render(GLFWwindow* target) {
        shader.use();

        if (texture.has_value()) {
            texture.value().bind();
        }
        
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    }
}