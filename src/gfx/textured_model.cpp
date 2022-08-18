#include "gfx/textured_model.h"
#include "gfx/ssbo.h"

namespace simpleengine::gfx {
    TexturedModel::TexturedModel(GLFWwindow* window, gfx::Shader shader, gfx::Texture texture, std::vector<LitVertex> vertices,
            std::vector<GLuint> indicies) : simpleengine::gfx::Model(window, shader, vertices, indicies), 
            textures(std::vector<gfx::Texture>{texture}) {
    }

    TexturedModel::TexturedModel(GLFWwindow* window, GLuint shader_program, gfx::Texture texture,
            std::vector<LitVertex> vertices, std::vector<GLuint> indicies) : TexturedModel(window, gfx::Shader(shader_program),
            std::vector<gfx::Texture>{texture}, vertices, indicies) {

    }

    TexturedModel::TexturedModel(GLFWwindow* window, gfx::Shader shader, std::vector<gfx::Texture> textures, std::vector<LitVertex> vertices,
            std::vector<GLuint> indicies) : simpleengine::gfx::Model(window, shader, vertices, indicies), 
            textures(textures) {
    }

    TexturedModel::TexturedModel(GLFWwindow* window, GLuint shader_program, std::vector<gfx::Texture> textures,
            std::vector<LitVertex> vertices, std::vector<GLuint> indicies) : TexturedModel(window, gfx::Shader(shader_program),
            textures, vertices, indicies) {

    }

    void TexturedModel::update(const float& delta_time) {
        
    }

    void TexturedModel::render(GLFWwindow* target) {
        shader.use();
        shader.set_uniform_matrix_4f("transform_matrix", transform_matrix, false);

        const int tex_id = 0;

        // On a batch renderer, you would set an array.
        shader.set_uniform_int("u_textures", tex_id, false);
        shader.set_uniform_float("u_texture_shine", 1.f, false);
        shader.set_uniform_float("u_texture_reflectivity", 0.f, false);

        /* int samples[2] = { 0, 1 };
        float shine[2] = { 1.f, 1.f};
        float reflectivity[2] = { 0.f, 0.f};

        shader.set_uniform_int_array("u_textures", 2, samples, false);
        shader.set_uniform_float_array("u_texture_shine", 2, shine, false);
        shader.set_uniform_float_array("u_texture_reflectivity", 2, reflectivity, false); */

        glActiveTexture(GL_TEXTURE0 + tex_id);
        glBindTextureUnit(tex_id, textures.front().get_texture_id());
        const auto& texture = textures.front();
        texture.bind();
        
        vao.bind();
        if (indicies.empty()) {
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        } else {
            glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
        }
    }
}