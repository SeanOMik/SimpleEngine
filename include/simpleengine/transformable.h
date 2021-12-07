#pragma once

#include <glm/detail/qualifier.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace simpleengine {
    class Transformable {
    protected:

    public:
        glm::mat4 transform_matrix;

        Transformable() : transform_matrix(glm::mat4(1.f)) {

        }

        Transformable(glm::mat4 transform_matrix) : transform_matrix(transform_matrix) {

        }

        friend Transformable operator+(Transformable lhs, const Transformable& rhs) {
            lhs.transform_matrix += rhs.transform_matrix;
            return lhs;
        }

        friend Transformable operator-(Transformable lhs, const Transformable& rhs) {
            lhs.transform_matrix -= rhs.transform_matrix;
            return lhs;
        }

        friend Transformable operator*(Transformable lhs, const Transformable& rhs) {
            lhs.transform_matrix *= rhs.transform_matrix;
            return lhs;
        }

        friend Transformable operator/(Transformable lhs, const Transformable& rhs) {
            lhs.transform_matrix /= rhs.transform_matrix;
            return lhs;
        }

        virtual void combine_transform(const glm::mat4& transform_matrix) {
            this->transform_matrix *= transform_matrix;
        }

        virtual void combine_transform(const Transformable& transformable) {
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
            transform_matrix = scalar * transform_matrix;
        }
    };
}