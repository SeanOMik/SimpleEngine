#include "camera.h"
#include <GLFW/glfw3.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/trigonometric.hpp>

namespace simpleengine {
    Camera::Camera(GLFWwindow* window, gfx::Shader shader, float fov, glm::vec3 position, glm::vec3 rotation,
            float near_plane, float far_plane, glm::vec3 world_up, glm::vec3 cam_front) : simpleengine::Event(window), shader(shader),
            projection_matrix(1.f), view_matrix(1.f), fov(fov), position(position), rotation(rotation), near_plane(near_plane), far_plane(far_plane) {
        
        // TODO: Update width and height on window resize.
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        projection_matrix = glm::perspective(glm::radians(fov), ((float) width) / height, near_plane, far_plane);

        //rotation = glm::vec3(0.f, 0.f, -1.f);
    }

    Camera::Camera(GLFWwindow* window, GLuint shader_prog, float fov, glm::vec3 position, glm::vec3 rotation,
            float near_plane, float far_plane, glm::vec3 world_up, glm::vec3 cam_front) : Camera(window, gfx::Shader(shader_prog), fov, position,
            rotation, near_plane, far_plane, world_up, cam_front) {

    }
    
    void Camera::update(const float& delta_time) {
        if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
            movement_speed -= abs(movement_speed - .2f);
        }

        if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
            movement_speed += .2f;
        }

        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            movement_speed = 2.5f;
        }

        float camera_speed = movement_speed * delta_time;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            position += camera_speed * camera_front;
        }
        
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            position -= camera_speed * camera_front;
        }
        
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            position += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
        }
        
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            position -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            position.y += camera_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            position.y -= camera_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            rotation.z += camera_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            rotation.z -= camera_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            rotation.y -= camera_speed;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            rotation.y += camera_speed;
        }

        // Limit the pitch of the camera.
        if (rotation.z >= glm::radians(89.f)) {
            rotation.z = glm::radians(89.f);
        }

        if (rotation.z <= -glm::radians(89.f)) {
            rotation.z = -glm::radians(89.f);
        }

        glm::vec3 direction;
        direction.x = cos(rotation.y) * cos(rotation.z);
        direction.y = sin(rotation.z);
        direction.z = sin(rotation.y) * cos(rotation.z);
        camera_front = glm::normalize(direction);

        view_matrix = glm::lookAt(position, position + camera_front, camera_up);

        shader.use();
        shader.set_uniform_matrix_4f("view_matrix", view_matrix, false);
        shader.set_uniform_matrix_4f("projection_matrix", projection_matrix, false);
        shader.unuse();
    }

    void Camera::render(GLFWwindow* target) {

    }
}