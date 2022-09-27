#pragma once

#include "shader.h"
#include "simpleengine/gfx/mesh.h"

#include <unordered_map>
#include <vector>
#include <queue>

namespace simpleengine::gfx {
    class RenderingJob {
    public:
        gfx::Mesh& rendering_mesh;
        glm::mat4 transform_mat;

        RenderingJob(gfx::Mesh& mesh, glm::mat4 position) : rendering_mesh(mesh), transform_mat(position) {

        }
    };

    class Renderer : public simpleengine::Renderable {
    private:
        GLFWwindow* window;
    public:

        std::queue<RenderingJob> rendering_queue;
        gfx::Shader shader;

        Renderer(GLFWwindow* window, gfx::Shader shader);
        Renderer(GLFWwindow* window, GLuint shader_program);

        void enable_debug();

        virtual void queue_job(RenderingJob job);
        virtual void create_job_buffers(RenderingJob& job);

        virtual void initialize();
        virtual void destroy() override;

        virtual void update(const float& delta_time) override;
        
        virtual void render() override;
    };
}