#include "gfx/renderer.h"

namespace simpleengine::gfx {
    Renderer::Renderer(GLFWwindow* window, gfx::Shader shader): window(window), shader(shader) {

    }

    Renderer::Renderer(GLFWwindow* window, GLuint shader_program): Renderer(window, 
        gfx::Shader(shader_program)) {

    }

    void Renderer::add_model(simpleengine::gfx::Texture texture, std::shared_ptr<simpleengine::gfx::Model> model) {
        auto ebo = gfx::VBO::init(GL_ELEMENT_ARRAY_BUFFER, false);
        auto vbo = gfx::VBO::init(GL_ARRAY_BUFFER, false);
        auto vao = gfx::VAO::init();

        // Create and setup the EBO, VAO, and VBOs.
        vao.bind();
        vbo.buffer(model->vertices.data(), 0, sizeof(LitVertex) * model->vertices.size());
        if (!model->indicies.empty()) {
            ebo.buffer(model->indicies.data(), 0, model->indicies.size() * sizeof(GLuint));
        }

        // Enable VAO attributes
        vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position));
        vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color));
        vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal));
        vao.enable_attrib(vbo, 3, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord));
        vao.enable_attrib(vbo, 4, 1, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, texture_id));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // Create the RenderingModel struct and store it in the vector for later.
        RenderingModel rendering_model( model, texture, ebo, vbo, vao);
        rendering_models.push_back(rendering_model);
    }

    void Renderer::remove_model(std::shared_ptr<simpleengine::gfx::Model> model) {
        std::cerr << "Removing model is unimplemented!" << std::endl;
    }

    void Renderer::initialize() {
        std::cout << "Base Renderer initialized" << std::endl;
    }

    void Renderer::render(GLFWwindow* target) {
        shader.use();

        for (RenderingModel& rendering : rendering_models) {
            std::shared_ptr<Model>& model = rendering.model;

            shader.use();
            shader.set_uniform_matrix_4f("transform_matrix", model->transform_matrix, false);
            
            rendering.vao.bind();
            if (model->indicies.empty()) {
                glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());
            } else {
                glDrawElements(GL_TRIANGLES, model->indicies.size(), GL_UNSIGNED_INT, 0);
            }

            /* const Texture& texture = rendering.texture;

            shader.set_uniform_matrix_4f("transform_matrix", model->transform_matrix, false);

            const int tex_id = 0;

            // On a batch renderer, you would set an array.
            shader.set_uniform_int("u_textures", tex_id, false);
            shader.set_uniform_float("u_texture_shine", 1.f, false);
            shader.set_uniform_float("u_texture_reflectivity", 0.f, false);

            glActiveTexture(GL_TEXTURE0 + tex_id);
            glBindTextureUnit(tex_id, texture.get_texture_id());
            texture.bind();
            
            rendering.vao.bind();
            if (model->indicies.empty()) {
                glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());
            } else {
                glDrawElements(GL_TRIANGLES, model->indicies.size(), GL_UNSIGNED_INT, 0);
            } */
        }

        shader.unuse();
    }
}