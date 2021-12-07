#include "camera.h"

namespace simpleengine {
    Camera::Camera(GLFWwindow* window, gfx::Shader shader, float fov, glm::vec3 position, glm::vec3 rotation,
            float near_plane, float far_plane, glm::vec3 world_up, glm::vec3 cam_front) : simpleengine::Event(window), shader(shader),
            projection_matrix(1.f), view_matrix(1.f), fov(fov), position(position), rotation(rotation), near_plane(near_plane), far_plane(far_plane),
            world_up(world_up), cam_front(cam_front) {
        
        // TODO: Update width and height on window resize.
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        projection_matrix = glm::perspective(glm::radians(fov), ((float) width) / height, near_plane, far_plane);
    }

    Camera::Camera(GLFWwindow* window, std::shared_ptr<GLuint> shader_prog, float fov, glm::vec3 position, glm::vec3 rotation,
            float near_plane, float far_plane, glm::vec3 world_up, glm::vec3 cam_front) : Camera(window, gfx::Shader(shader_prog), fov, position,
            rotation, near_plane, far_plane, world_up, cam_front) {

    }
    
    void Camera::update(const float& delta_time) {
        /* if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position.z -= 0.02f;
        }
        
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position.z += 0.02f;
        }
        
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position.x += 0.02f;
        }
        
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position.x -= 0.02f;
        } */

        view_matrix = glm::lookAt(position, position + cam_front, world_up);

        shader.use();
        shader.set_uniform_matrix_4f("view_matrix", view_matrix, false);
        shader.set_uniform_matrix_4f("projection_matrix", projection_matrix, false);
        shader.unuse();
    }

    void Camera::render(GLFWwindow* target) {

    }
}