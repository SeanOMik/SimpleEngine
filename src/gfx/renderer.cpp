#include "gfx/renderer.h"
#include "gfx/mesh.h"
#include "gfx/vao.h"
#include "renderable.h"

#include "ecs/component/mesh_component.h"
#include "ecs/component/model_component.h"
#include "vector.h"

#include <algorithm>
#include <assimp/material.h>
#include <functional>
#include <glm/geometric.hpp>
#include <stdexcept>

namespace simpleengine::gfx {
    void create_mesh_buffers(simpleengine::gfx::Mesh &mesh);

    RenderingJob::RenderingJob(RenderingType rendering_type, gfx::Mesh &mesh, glm::mat4 position)
        : rendering_type(rendering_type), rendering_mesh(&mesh), transform_mat(position) {}

    Renderer::Renderer(GLFWwindow *window, gfx::Shader shader, std::shared_ptr<Camera> camera)
        : window(window), shader(shader), camera(camera)/* , transparent_render_queue(CameraDistanceComparator(camera)) */ {}

    Renderer::Renderer(GLFWwindow *window, GLuint shader_program, std::shared_ptr<Camera> camera)
        : Renderer(window, gfx::Shader(shader_program), camera) {}

    void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar *message, const void *userParam) {

        fprintf(stderr, "%s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
    }

    void Renderer::check_if_initialized() {
        if (!is_initialized) {
            std::cerr << "Renderer is not initialized!" << std::endl;
            throw std::runtime_error("Renderer is not initialized!");
        }
    }

    void Renderer::enable_debug() {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debug_message_callback, 0);
    }

    void Renderer::sort_jobs() {
        // Sort transparents

        // std::sort()
    }

    void Renderer::queue_job(RenderingType rendering_type, gfx::Mesh &mesh, glm::mat4 position) {
        RenderingJob job(rendering_type, mesh, position);

        this->queue_job(job);
    }

    void Renderer::queue_job(RenderingJob job) {
        create_job_buffers(job);

        switch (job.rendering_type) {
        case RenderingType::RendType_TRANSPARENT: {
            /* glm::vec3 pos = job.transform_mat[3];
            float distance = glm::distance(pos, camera->position); */
            this->transparent_render_queue.emplace(job);
            break;
        }
        default:
            this->other_render_queue.emplace(job);
            break;
        }

        sort_jobs();
    }

    void Renderer::create_job_buffers(RenderingJob &job) {
        Mesh *rendering_mesh = job.rendering_mesh;

        if (!rendering_mesh->are_buffers_created) {
            gfx::VBO &vbo = rendering_mesh->vbo;
            gfx::VBO &ebo = rendering_mesh->ebo;
            gfx::VAO &vao = rendering_mesh->vao;

            vao.bind();
            vbo.buffer(rendering_mesh->vertices.data(), 0, sizeof(LitVertex) * rendering_mesh->vertices.size());

            if (!rendering_mesh->indicies.empty()) {
                ebo.buffer(rendering_mesh->indicies.data(), 0, rendering_mesh->indicies.size() * sizeof(GLuint));
            }

            // Enable VAO attributes
            vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position));
            vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color));
            vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal));
            vao.enable_attrib(vbo, 3, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord));

            rendering_mesh->tangent_vbo.buffer(rendering_mesh->tangents.data(), 0,
                                               rendering_mesh->tangents.size() * sizeof(Vectorf));
            vao.enable_attrib(rendering_mesh->tangent_vbo, 4, 3, GL_FLOAT, sizeof(Vectorf), 0);

            vbo.unbind();
            vao.unbind();

            rendering_mesh->are_buffers_created = true;

            std::cout << "Created render job buffers" << std::endl;
        }
    }

    void Renderer::update(const float &delta_time) {
        check_if_initialized();
    }

    void Renderer::initialize() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->is_initialized = true;

        std::cout << "Base Renderer initialized" << std::endl;
    }

    void Renderer::destroy() {
        std::cout << "Destroying renderer..." << std::endl;

        /* for (auto& [handle, rendering] : rendering_models) {
            rendering.destroy_buffers();
        } */
    }

    bool Renderer::render_job(const RenderingJob &job) {
        Mesh *mesh = job.rendering_mesh;

        shader.set_uniform_matrix_4f("u_transform_matrix", job.transform_mat);

        std::optional<Material> &material = mesh->material;

        if (material.has_value()) {
            shader.set_uniform_float("u_material.ambient_strength", material->ambient_strength);
            shader.set_uniform_float("u_material.diffuse_strength", material->diffuse_strength);
            shader.set_uniform_float("u_material.specular_strength", material->specular_strength);
            shader.set_uniform_float("u_material.shine_factor", material->shine_factor);
            // shader.set_uniform_float("u_material.reflect_factor", .1f);

            auto diffuse_maps = material->textures.find(aiTextureType_DIFFUSE);
            auto diffuse_map = diffuse_maps->second.front();

            shader.set_uniform_int("u_material.diffuse", 0);

            glActiveTexture(GL_TEXTURE0);
            diffuse_map->bind();

            // Apply the specular map if it exists
            auto specular_maps = material->textures.find(aiTextureType_SPECULAR);
            if (specular_maps != material->textures.end()) {
                auto spec = specular_maps->second.front();

                shader.set_uniform_int("u_material.has_specular_map", 1);
                shader.set_uniform_int("u_material.specular_map", 1);

                glActiveTexture(GL_TEXTURE1);
                spec->bind();
            } else {
                shader.set_uniform_int("u_material.has_specular_map", 0);
            }

            // Apply the normal map if it exists
            auto normal_maps = material->textures.find(aiTextureType_NORMALS);
            if (normal_maps != material->textures.end()) {
                auto normal = normal_maps->second.front();

                shader.set_uniform_int("u_material.has_normal_map", 1);
                shader.set_uniform_int("u_material.normal_map", 2);

                glActiveTexture(GL_TEXTURE2);
                normal->bind();
            } else {
                shader.set_uniform_int("u_material.has_normal_map", 0);
            }
        }

        mesh->vao.bind();
        if (mesh->indicies.empty()) {
            glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
        } else {
            glDrawElements(GL_TRIANGLES, mesh->indicies.size(), GL_UNSIGNED_INT, 0);
        }

        return true;
    }

    void Renderer::render_job_queue(std::queue<RenderingJob> &rendering_queue) {
        while (!rendering_queue.empty()) {
            // Get the job from the queue, we'll remove it after we render.
            RenderingJob &job = rendering_queue.front();

            bool res = this->render_job(job);

            if (res) {
                // Now we'll remove the job from the queue.
                rendering_queue.pop();
            }
        }
    }

    void Renderer::render_job_queue(std::map<float, RenderingJob, std::greater<>>& rendering_queue) {
        // Render each job then clear the queue
        for (const auto& it : rendering_queue) {
            this->render_job(it.second);
        }
        rendering_queue.clear();
    }

    void Renderer::render() {
        check_if_initialized();

        // Render other (opaque) objects first 
        render_job_queue(other_render_queue);

        // Render transparent objects
        std::map<float, RenderingJob, std::greater<>> transparent_jobs;
        while (!transparent_render_queue.empty()) {
            RenderingJob& job = transparent_render_queue.front();

            glm::vec3 pos = job.transform_mat[3];
            float distance = glm::distance(pos, camera->position);
            transparent_jobs.emplace(distance, job);

            transparent_render_queue.pop();
        }
        render_job_queue(transparent_jobs);
    }
} // namespace simpleengine::gfx