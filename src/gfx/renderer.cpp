#include "gfx/renderer.h"
#include "ecs/component/component.h"
#include "ecs/entity.h"
#include "gfx/mesh.h"
#include "gfx/vao.h"
#include "renderable.h"

#include "ecs/component/mesh_component.h"
#include "ecs/component/model_component.h"

#include <algorithm>
#include <assimp/material.h>

namespace simpleengine::gfx {
    void create_mesh_buffers(std::shared_ptr<simpleengine::Component> comp, simpleengine::gfx::Mesh& mesh);

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

    void Renderer::render() {
        shader.use();
        
        for (auto& [handle, rendering] : rendering_models) {
            if (rendering.component_models.size() >= 0) {
                std::shared_ptr<Entity>& entity = rendering.entity;

                shader.set_uniform_matrix_4f("transform_matrix", entity->transform_matrix, false);

                for (const auto& pair : rendering.component_models) {
                    Mesh& model = pair.second;
                    std::optional<Material>& material = model.material;

                    shader.set_uniform_int("u_textures", 0, false);

                    if (material.has_value()) {
                        shader.set_uniform_float("u_texture_shine", material->shine, false);
                        shader.set_uniform_float("u_texture_reflectivity", material->reflectivity, false);

                        int texture_count = 0;
                        auto diffuse_maps = material->textures.find(aiTextureType_DIFFUSE);
                        for (const auto& texture : diffuse_maps->second) {
                            // We can only bind to 16 textures at a time (indexes are 0-15)
                            if (texture_count >= 16) break;

                            glActiveTexture(GL_TEXTURE0 + texture_count);
                            glBindTextureUnit(texture_count, texture.get_texture_id());

                            texture_count++;
                        }
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

    void Renderer::RenderingModel::update_buffers() {
        if (entity->has_component<simpleengine::MeshComponent>()) {
            std::shared_ptr<MeshComponent> comp = entity->get_component<simpleengine::MeshComponent>();
            auto iter = component_models.find(comp->get_handle());
            if (iter == component_models.end()) {
                std::cout << "Enabling buffer attributes for MeshComponent (" << comp->get_handle() << ")..." << std::endl;

                //iter->second = comp->model;
                gfx::Mesh& mesh = comp->model;
                create_mesh_buffers(comp, mesh);
                component_models.emplace(comp->get_handle(), mesh);

                std::cout << "Enabled all buffer attributes for MeshComponent" << std::endl;
            } else {
                std::cout << "Already exists" << std::endl;
            }
        } else if (entity->has_component<simpleengine::ModelComponent>()) {
            std::shared_ptr<ModelComponent> comp = entity->get_component<simpleengine::ModelComponent>();

            auto iter = component_models.find(comp->get_handle());
            if (iter == component_models.end()) {
                std::cout << "Enabling buffer attributes for ModelComponent (" << comp->get_handle() << ")..." << std::endl;
                
                // Store all the model's meshes
                for (auto& mesh : comp->model.meshes) {
                    create_mesh_buffers(comp, mesh);

                    component_models.emplace(comp->get_handle(), mesh);
                }

                std::cout << "Enabled all buffer attributes for ModelComponent" << std::endl;
            } else {
                std::cout << "Already exists" << std::endl;
            }
        }
    }

    void create_mesh_buffers(std::shared_ptr<Component> comp, gfx::Mesh& mesh) {
        gfx::VBO& vbo = mesh.vbo;
        gfx::VBO& ebo = mesh.ebo;
        gfx::VAO& vao = mesh.vao;

        vao.bind();
        vbo.buffer(mesh.vertices.data(), 0, sizeof(LitVertex) * mesh.vertices.size());

        if (!mesh.indicies.empty()) {
            ebo.buffer(mesh.indicies.data(), 0, mesh.indicies.size() * sizeof(GLuint));
        }

        // Enable VAO attributes
        vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position), false);
        vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color), false);
        vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal), false);
        vao.enable_attrib(vbo, 3, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord), false);
        vao.enable_attrib(vbo, 4, 1, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, texture_id), false);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}