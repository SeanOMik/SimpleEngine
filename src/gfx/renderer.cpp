#include "gfx/renderer.h"
#include "ecs/component/component.h"
#include "ecs/entity.h"
#include "gfx/model.h"
#include "renderable.h"

#include "ecs/component/model_componenet.h"

#include <algorithm>

namespace simpleengine::gfx {
    void Renderer::RenderingModel::update_buffers() {
        if (std::shared_ptr<ModelComponent> comp = entity->get_component<simpleengine::ModelComponent>()) {
            auto iter = rendering_buffers.find(comp->get_handle());
            if (iter == rendering_buffers.end()) {
                std::cout << "Creating buffer for ModelComponent (" << comp->get_handle() << ")..." << std::endl;

                auto ebo = gfx::VBO::init(GL_ELEMENT_ARRAY_BUFFER, false);
                auto vbo = gfx::VBO::init(GL_ARRAY_BUFFER, false);
                auto vao = gfx::VAO::init();
                auto& model = comp->model;

                // Create and setup the EBO, VAO, and VBOs.
                vao.bind();
                vbo.buffer(model.vertices.data(), 0, sizeof(LitVertex) * model.vertices.size());
                if (!model.indicies.empty()) {
                    ebo.buffer(model.indicies.data(), 0, sizeof(GLuint) * model.indicies.size());
                }

                // Enable VAO attributes
                vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position));
                vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color));
                vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal));
                vao.enable_attrib(vbo, 3, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord));
                vao.enable_attrib(vbo, 4, 1, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, texture_id));

                RenderingBuffers buffers(comp->model, ebo, vbo, vao);
                rendering_buffers.emplace(comp->get_handle(), buffers);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);

                std::cout << "Finished creating ModelComponent buffers!" << std::endl;
            } else {
                std::cout << "Already exists" << std::endl;
            }
        }
    }

    void Renderer::RenderingModel::destroy_buffers() {
        std::cout << "Destroying buffers for entity!" << std::endl;

        // Iterate through all buffer lists and destroy each inner buffer.
        for (auto& pair : rendering_buffers) {
            RenderingBuffers& buffers = pair.second;

            buffers.ebo.destroy();
            buffers.vao.destroy();
            buffers.vbo.destroy();
        }
    }

    Renderer::Renderer(GLFWwindow* window, gfx::Shader shader): window(window), shader(shader) {
        
    }

    Renderer::Renderer(GLFWwindow* window, GLuint shader_program): Renderer(window, 
        gfx::Shader(shader_program)) {

    }

    void Renderer::submit_entity(std::shared_ptr<simpleengine::Entity> entity) {
        std::cout << "Submitting entity (" << entity->get_handle() << ")..." << std::endl;
        auto it = rendering_models.emplace(entity->get_handle(), entity);
        it.first->second.update_buffers();
    }

    bool Renderer::withdraw_entity(std::shared_ptr<simpleengine::Entity> entity) {
        std::cout << "Withdrawing entity (" << entity->get_handle() << ")...";
        auto it = rendering_models.find(entity->get_handle());

        if (it != rendering_models.end()) {
            it->second.destroy_buffers();
            rendering_models.erase(it);

            return true;
        }

        return false;
    }

    void Renderer::update(const float& delta_time) {
        this->render(nullptr);
    }

    void Renderer::initialize() {
        std::cout << "Base Renderer initialized" << std::endl;
    }

    void Renderer::destroy() {
        std::cout << "Destroying renderer..." << std::endl;

        for (auto& [handle, rendering] : rendering_models) {
            rendering.destroy_buffers();
            //rendering.entity->destroy();
        }
    }

    void Renderer::render(GLFWwindow* target) {
        shader.use();
        
        for (auto& [handle, rendering] : rendering_models) {
            if (rendering.rendering_buffers.size() > 0) {
                std::shared_ptr<Entity>& entity = rendering.entity;

                shader.set_uniform_matrix_4f("transform_matrix", entity->transform_matrix, false);
                
                for (const auto& pair : rendering.rendering_buffers) {
                    const RenderingBuffers& buffers = pair.second;
                    Model& model = buffers.model;
                    std::optional<Material>& material = model.material;

                    shader.set_uniform_int("u_textures", 0, false);

                    if (material.has_value()) {
                        //Material& material = material
                        shader.set_uniform_float("u_texture_shine", material->shine, false);
                        shader.set_uniform_float("u_texture_reflectivity", material->reflectivity, false);

                        glActiveTexture(GL_TEXTURE0);
                        glBindTextureUnit(0, material->texture.get_texture_id());
                        material->texture.bind();
                    }
                    
                    buffers.vao.bind();
                    if (model.indicies.empty()) {
                        glDrawArrays(GL_TRIANGLES, 0, model.vertices.size());
                    } else {
                        glDrawElements(GL_TRIANGLES, model.indicies.size(), GL_UNSIGNED_INT, 0);
                    }
                }
            }
        }

        shader.unuse();
    }
}