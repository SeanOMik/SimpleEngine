#include "gfx/textured_model.h"

namespace simpleengine::gfx {
    TexturedModel::TexturedModel(GLFWwindow* window, gfx::Shader shader, gfx::Texture texture, std::vector<Vertex> vertices,
            std::vector<GLuint> indicies) : simpleengine::gfx::Model(window, shader, vertices, indicies), texture(texture) {

    }

    TexturedModel::TexturedModel(GLFWwindow* window, std::shared_ptr<GLuint> shader_program, gfx::Texture texture,
            std::vector<Vertex> vertices, std::vector<GLuint> indicies) : TexturedModel(window, gfx::Shader(shader_program),
            texture, vertices, indicies) {

    }

    void TexturedModel::update(const float& delta_time) {
        
    }

    void TexturedModel::render(GLFWwindow* target) {
        shader.use();
        shader.set_uniform_matrix_4f("transform_matrix", transform_matrix, false);
        shader.set_uniform_float("shine_damper", texture.shine_damper, false);
        shader.set_uniform_float("reflectivity", texture.reflectivity, false);

        // When binding to the texture, tell the shader if the texture is set or not.
        shader.set_uniform_int("texture_is_set", (GLint) true, false);
        texture.bind();
        
        vao.bind();
        if (indicies.empty()) {
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        } else {
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
        }
    }
}