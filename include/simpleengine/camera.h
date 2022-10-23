#pragma once

#include "simpleengine/gfx/shader.h"
#include "event/event.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace simpleengine {
    class Camera : public simpleengine::Event {
    private:
        GLFWwindow* window;
    public:

        glm::vec3 position;
        glm::vec3 rotation;
        gfx::Shader shader;

        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
        glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

        float fov;
        float near_plane;
        float far_plane;
        float movement_speed = 2.5f;

        Camera(GLFWwindow* window, gfx::Shader shader, float fov = 70, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f),
            float near_plane = 0.1f, float far_plane = 1000.f);

        Camera(GLFWwindow* window, GLuint shader_prog, float fov = 70, glm::vec3 position = glm::vec3(0.f),
            glm::vec3 rotation = glm::vec3(0.f), float near_plane = 0.1f, float far_plane = 1000.f);

        virtual void update(const float& delta_time) override;
        virtual void input_update(const float& delta_time) override;
    };
}