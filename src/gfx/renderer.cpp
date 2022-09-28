#include "gfx/renderer.h"
#include "gfx/mesh.h"
#include "gfx/vao.h"
#include "renderable.h"

#include "ecs/component/mesh_component.h"
#include "ecs/component/model_component.h"

#include <algorithm>
#include <assimp/material.h>

namespace simpleengine::gfx {
    void create_mesh_buffers(simpleengine::gfx::Mesh& mesh);

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

    void Renderer::queue_job(RenderingJob job) {
        RenderingJob& emplace = rendering_queue.emplace(job);
        create_job_buffers(emplace);
    }

    void Renderer::create_job_buffers(RenderingJob& job) {
        Mesh& rendering_mesh = job.rendering_mesh;

        if (!rendering_mesh.are_buffers_created) {
            gfx::VBO& vbo = rendering_mesh.vbo;
            gfx::VBO& ebo = rendering_mesh.ebo;
            gfx::VAO& vao = rendering_mesh.vao;

            vao.bind();
            vbo.buffer(rendering_mesh.vertices.data(), 0, sizeof(LitVertex) * rendering_mesh.vertices.size());

            if (!rendering_mesh.indicies.empty()) {
                ebo.buffer(rendering_mesh.indicies.data(), 0, rendering_mesh.indicies.size() * sizeof(GLuint));
            }

            // Enable VAO attributes
            vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position), false);
            vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color), false);
            vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal), false);
            vao.enable_attrib(vbo, 3, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord), false);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            rendering_mesh.are_buffers_created = true;

            std::cout << "Created render job buffers" << std::endl;
        }
    }

    void Renderer::update(const float& delta_time) {
        
    }

    void Renderer::initialize() {
        std::cout << "Base Renderer initialized" << std::endl;
    }

    void Renderer::destroy() {
        std::cout << "Destroying renderer..." << std::endl;

        /* for (auto& [handle, rendering] : rendering_models) {
            rendering.destroy_buffers();
        } */
    }

    void Renderer::render() {
        shader.use();
        
        while (!rendering_queue.empty()) {
            // Get the job from the queue, we'll remove it after we render.
            RenderingJob& job = rendering_queue.front();
            Mesh& mesh = job.rendering_mesh;

            shader.set_uniform_matrix_4f("u_transform_matrix", job.transform_mat, false);

            std::optional<Material>& material = mesh.material;

            if (material.has_value()) {
                shader.set_uniform_float("u_material.ambient_strength", material->ambient_strength, false);
                shader.set_uniform_float("u_material.diffuse_strength", material->diffuse_strength, false);
                shader.set_uniform_float("u_material.specular_strength", material->specular_strength, false);
                shader.set_uniform_float("u_material.shine_factor", material->shine_factor, false);
                //shader.set_uniform_float("u_material.reflect_factor", .1f, false);

                auto diffuse_maps = material->textures.find(aiTextureType_DIFFUSE);
                auto diffuse_map = diffuse_maps->second.front();

                shader.set_uniform_int("u_material.diffuse", 0, false);

                glActiveTexture(GL_TEXTURE0);
                diffuse_map->bind();

                auto specular_maps = material->textures.find(aiTextureType_SPECULAR);

                
                if (specular_maps != material->textures.end()) {
                    auto spec = specular_maps->second.front();

                    shader.set_uniform_int("u_material.specular_map", 1, false);

                    glActiveTexture(GL_TEXTURE1);
                    spec->bind();
                }

                //diffuse_map
                /* for (const auto& texture : diffuse_maps->second) {
                    // We can only bind to 16 textures at a time (indexes are 0-15)
                    if (texture_count >= 16) break;

                    glActiveTexture(GL_TEXTURE0 + texture_count);
                    glBindTextureUnit(texture_count, texture->get_texture_id());

                    texture_count++;
                } */
            }
            
            mesh.vao.bind();
            if (mesh.indicies.empty()) {
                glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
            } else {
                glDrawElements(GL_TRIANGLES, mesh.indicies.size(), GL_UNSIGNED_INT, 0);
            }
            
            // Now we'll remove the job from the queue.
            rendering_queue.pop();
        }

        shader.unuse();
    }
}