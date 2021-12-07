#pragma once

#include "simpleengine/gfx/shader.h"
#include "event/event.h"

#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace simpleengine {
    class Camera : public simpleengine::Event {
    private:

    public:
        glm::vec3 position;
        glm::vec3 rotation;
        gfx::Shader shader;

        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
        glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cam_front = glm::vec3(0.f, 0.f, -1.f);

        float fov;
        float near_plane ;
        float far_plane;

        Camera(GLFWwindow* window, gfx::Shader shader, float fov = 70, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f),
            float near_plane = 0.1f, float far_plane = 1000.f, glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f), 
            glm::vec3 cam_front = glm::vec3(0.f, 0.f, -1.f));

        Camera(GLFWwindow* window, std::shared_ptr<GLuint> shader_prog, float fov = 70, glm::vec3 position = glm::vec3(0.f),
            glm::vec3 rotation = glm::vec3(0.f), float near_plane = 0.1f, float far_plane = 1000.f, glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f), 
            glm::vec3 cam_front = glm::vec3(0.f, 0.f, -1.f));
        
        virtual void update(const float& delta_time) override;
        virtual void render(GLFWwindow* target) override;
    };
}