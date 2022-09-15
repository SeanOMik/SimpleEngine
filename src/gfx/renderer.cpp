#include "gfx/renderer.h"
#include "ecs/component/component.h"
#include "ecs/entity.h"
#include "gfx/model.h"
#include "gfx/vao.h"
#include "renderable.h"

#include "ecs/component/model_componenet.h"

#include <algorithm>

namespace simpleengine::gfx {
    void Renderer::RenderingModel::update_buffers() {
        if (std::shared_ptr<ModelComponent> comp = entity->get_component<simpleengine::ModelComponent>()) {
            auto iter = component_models.find(comp->get_handle());
            if (iter == component_models.end()) {
                std::cout << "Enabling buffer attributes for ModelComponent (" << comp->get_handle() << ")..." << std::endl;

                //iter->second = comp->model;
                gfx::Model& model = comp->model;
                gfx::VBO& vbo = model.vbo;
                gfx::VBO& ebo = model.ebo;
                gfx::VAO& vao = model.vao;

                vao.bind();
                vbo.buffer(model.vertices.data(), 0, sizeof(LitVertex) * model.vertices.size());

                if (!model.indicies.empty()) {
                    ebo.buffer(model.indicies.data(), 0, model.indicies.size() * sizeof(GLuint));
                }

                // Enable VAO attributes
                vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position), false);
                vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color), false);
                vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal), false);
                vao.enable_attrib(vbo, 3, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord), false);
                vao.enable_attrib(vbo, 4, 1, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, texture_id), false);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);

                component_models.emplace(comp->get_handle(), model);

                std::cout << "Enabled all buffer attributes for ModelComponent" << std::endl;
            } else {
                std::cout << "Already exists" << std::endl;
            }
        }
    }

    void Renderer::RenderingModel::destroy_buffers() {
        std::cout << "Destroying entity models..." << std::endl;

        // Iterate through all buffer lists and destroy each inner buffer.
        for (auto& pair : component_models) {
            pair.second.destroy();
        }
    }

    Renderer::Renderer(GLFWwindow* window, gfx::Shader shader): window(window), shader(shader) {
        
    }

    Renderer::Renderer(GLFWwindow* window, GLuint shader_program): Renderer(window, 
        gfx::Shader(shader_program)) {

    }

    void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar* message, const void* userParam) {
        
        fprintf(stderr, "%s type = 0x%x, severity = 0x%x, message = %s\n",
           (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
    }

    void Renderer::enable_debug() {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debug_message_callback, 0);
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
        }
    }

    void Renderer::render(GLFWwindow* target) {
        shader.use();
        
        for (auto& [handle, rendering] : rendering_models) {
            if (rendering.component_models.size() > 0) {
                std::shared_ptr<Entity>& entity = rendering.entity;

                shader.set_uniform_matrix_4f("transform_matrix", entity->transform_matrix, false);
                
                for (const auto& pair : rendering.component_models) {
                    Model& model = pair.second;
                    std::optional<Material>& material = model.material;

                    shader.set_uniform_int("u_textures", 0, false);

                    if (material.has_value()) {
                        shader.set_uniform_float("u_texture_shine", material->shine, false);
                        shader.set_uniform_float("u_texture_reflectivity", material->reflectivity, false);

                        glActiveTexture(GL_TEXTURE0);
                        glBindTextureUnit(0, material->texture.get_texture_id());
                        material->texture.bind();
                    }
                    
                    model.vao.bind();
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