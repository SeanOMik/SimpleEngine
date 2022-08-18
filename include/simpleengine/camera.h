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
        //glm::vec3 camera_forward;
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        gfx::Shader shader;

        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
        //glm::vec3 camera_rotation;
        /* glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 cam_front = glm::vec3(0.f, 0.f, -1.f); */
        glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

        float fov;
        float near_plane;
        float far_plane;
        float movement_speed = 2.5f;

        Camera(GLFWwindow* window, gfx::Shader shader, float fov = 70, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f),
            float near_plane = 0.1f, float far_plane = 1000.f, glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f), 
            glm::vec3 cam_front = glm::vec3(0.f, 0.f, -1.f));

        Camera(GLFWwindow* window, GLuint shader_prog, float fov = 70, glm::vec3 position = glm::vec3(0.f),
            glm::vec3 rotation = glm::vec3(0.f), float near_plane = 0.1f, float far_plane = 1000.f, glm::vec3 world_up = glm::vec3(0.f, 1.f, 0.f), 
            glm::vec3 cam_front = glm::vec3(0.f, 0.f, -1.f));
        
        virtual void update(const float& delta_time) override;
        virtual void render(GLFWwindow* target) override;
    };
}