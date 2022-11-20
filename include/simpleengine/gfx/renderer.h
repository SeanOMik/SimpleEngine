#pragma once

#include "../camera.h"
#include "../log/logger.h"
#include "shader.h"
#include "../renderable.h"
#include "rendering_type.h"

#include <unordered_map>
#include <vector>
#include <queue>
#include <map>

namespace simpleengine::gfx {
    class Mesh;

    class RenderingJob {
    public:
        RenderingType rendering_type;
        gfx::Mesh* rendering_mesh;
        glm::mat4 last_transform_mat;
        glm::mat4 transform_mat;

        RenderingJob(RenderingType rendering_type, gfx::Mesh& mesh, glm::mat4 last_pos, glm::mat4 position);
    };

    class Renderer : public simpleengine::Renderable {
    private:
        GLFWwindow* window;
    protected:
        bool is_initialized = false;

        void check_if_initialized();
        simpleengine::log::Logger logger;
    public:
        std::queue<RenderingJob> transparent_render_queue;
        std::queue<RenderingJob> other_render_queue;

        gfx::Shader shader;
        std::shared_ptr<Camera> camera;

        Renderer(GLFWwindow* window, gfx::Shader shader, std::shared_ptr<Camera> camera);
        Renderer(GLFWwindow* window, GLuint shader_program, std::shared_ptr<Camera> camera);

        void enable_debug();
        void enable_gl_option(GLenum option) const;

        virtual void sort_jobs();
        virtual void queue_job(RenderingType rendering_type, gfx::Mesh& mesh, glm::mat4 last_position, glm::mat4 position);
        virtual void queue_job(RenderingJob job);
        virtual void create_job_buffers(RenderingJob& job);

        virtual void initialize();
        virtual void destroy() override;

        virtual void update(const float& delta_time) override;
        
        virtual void render(const float& interpolate_alpha, const float& frame_time) override;

        /**
         * @brief Renders a single job.
         * 
         * @param job The job that will be rendered.
         * @return true if the job was rendered successfully.
         * @return false if there was an error when trying to render the job.
         */
        virtual bool render_job(const float& interpolate_alpha, const RenderingJob& job);
        virtual void render_job_queue(const float& interpolate_alpha, std::queue<RenderingJob>& queue);
        virtual void render_job_queue(const float& interpolate_alpha, std::map<float, RenderingJob, std::greater<>>& queue);
    };
}