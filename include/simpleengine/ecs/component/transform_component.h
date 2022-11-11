#pragma once

#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace simpleengine::ecs {
    namespace system {
        class SceneSystem;
    }

    /**
     * @brief A component that contains a Mesh that will be rendered.
     * 
     */
    class TransformComponent {
    friend simpleengine::ecs::system::SceneSystem;
    public:
        // This is the transform from the last render loop. The renderer uses this for frame interprelation
        glm::mat4 last_transform_matrix;
        glm::mat4 transform_matrix;

        TransformComponent() : transform_matrix(1.f), last_transform_matrix(1.f) {
            
        }

        TransformComponent(glm::vec3 initial_position, glm::vec3 initial_rotation = glm::vec3(0.f), glm::vec3 initial_scale = glm::vec3(1.f))
                : transform_matrix(1.f), last_transform_matrix(1.f) {
            
            translate(initial_position);
            
            rotate_x(initial_rotation.x);
            rotate_y(initial_rotation.y);
            rotate_z(initial_rotation.z);

            scale(initial_scale);
        }

        TransformComponent(glm::mat4 transform_matrix) : transform_matrix(transform_matrix), last_transform_matrix(1.f) {

        }

        friend TransformComponent operator+(TransformComponent lhs, const TransformComponent& rhs) {
            lhs.transform_matrix += rhs.transform_matrix;
            return lhs;
        }

        friend TransformComponent operator-(TransformComponent lhs, const TransformComponent& rhs) {
            lhs.transform_matrix -= rhs.transform_matrix;
            return lhs;
        }

        friend TransformComponent operator*(TransformComponent lhs, const TransformComponent& rhs) {
            lhs.transform_matrix *= rhs.transform_matrix;
            return lhs;
        }

        friend TransformComponent operator/(TransformComponent lhs, const TransformComponent& rhs) {
            lhs.transform_matrix /= rhs.transform_matrix;
            return lhs;
        }

        virtual void decompose_matrix(glm::vec3& pos, glm::quat& rot, glm::vec3& scale) const {
            pos = transform_matrix[3];
            for(int i = 0; i < 3; i++) {
                scale[i] = glm::length(glm::vec3(transform_matrix[i]));
            }

            const glm::mat3 rotMtx(glm::vec3(transform_matrix[0]) / scale[0],
                glm::vec3(transform_matrix[1]) / scale[1], glm::vec3(transform_matrix[2]) / scale[2]);
            rot = glm::quat_cast(rotMtx);
        }

        virtual glm::vec3 get_pos() const {
            return transform_matrix[3];
        }

        virtual glm::vec3 get_scale() const {
            glm::vec3 scale;
            for(int i = 0; i < 3; i++) {
                scale[i] = glm::length(glm::vec3(transform_matrix[i]));
            }
            
            return scale;
        }

        virtual glm::quat get_rotation_quat() const {
            glm::vec3 scale = get_scale();

            const glm::mat3 rot_mtx(glm::vec3(transform_matrix[0]) / scale[0],
                glm::vec3(transform_matrix[1]) / scale[1], glm::vec3(transform_matrix[2]) / scale[2]);
            return glm::quat_cast(rot_mtx);
        }

        virtual void combine_transform(const glm::mat4& transform_matrix) {
            this->transform_matrix *= transform_matrix;
        }

        virtual void combine_transform(const TransformComponent& transformable) {
            transform_matrix = transformable.transform_matrix;
        }

        virtual void translate(float x, float y, float z) {
            transform_matrix = glm::translate(transform_matrix, glm::vec3(x, y, z));
        }

        virtual void translate(const glm::vec3& vec) {
            transform_matrix = glm::translate(transform_matrix, vec);
        }

        virtual glm::mat4 rotation_matrix(float degrees, glm::vec3 rotation_axis) const {
            return glm::rotate(transform_matrix, glm::radians(degrees), rotation_axis);
        }

        virtual glm::mat4 rotation_x_matrix(float degrees) const {
            return rotation_matrix(degrees, glm::vec3(1, 0, 0));
        }

        virtual glm::mat4 rotation_y_matrix(float degrees) const {
            return rotation_matrix(degrees, glm::vec3(0, 1, 0));
        }

        virtual glm::mat4 rotation_z_matrix(float degrees) const {
            return rotation_matrix(degrees, glm::vec3(0, 0, 1));
        }

        virtual void rotate(float degrees, glm::vec3 rotation_axis) {
            transform_matrix = rotation_matrix(degrees, rotation_axis);
        }

        virtual void rotate_x(float degrees) {
            transform_matrix = rotation_x_matrix(degrees);
        }
        
        virtual void rotate_y(float degrees) {
            transform_matrix = rotation_y_matrix(degrees);
        }

        virtual void rotate_z(float degrees) {
            transform_matrix = rotation_z_matrix(degrees);
        }

        virtual void scale(glm::vec3 scalar_vec) {
            transform_matrix = glm::scale(transform_matrix, scalar_vec);
        }

        virtual void scale(float scalar) {
            transform_matrix = glm::scale(transform_matrix, glm::vec3(scalar, scalar, scalar));
        }
    };
}