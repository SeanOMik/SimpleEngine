#pragma once

#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace simpleengine {
    template<typename VectorType = float>
    class Vector {
    private:
        glm::vec<3, VectorType, glm::defaultp> inner_vec;
    public:
        Vector() = default;

        Vector(VectorType x, VectorType y, VectorType z) : inner_vec(x, y, z) {

        }

        /* Vector(glm::vec<3, VectorType, glm::defaultp> glm_vec) : inner_vec(glm_vec) {

        } */

        Vector(const glm::vec<3, VectorType, glm::defaultp>& glm_vec) : inner_vec(glm_vec) {

        }

        operator glm::vec<3, VectorType, glm::defaultp>() const {
            return inner_vec;
        }

        VectorType& x() {
            return inner_vec.x;
        }

        VectorType& y() {
            return inner_vec.y;
        }

        VectorType& z() {
            return inner_vec.z;
        }

        void set_x(const VectorType x) {
            inner_vec.x = x;
        }

        void set_y(const VectorType y) {
            inner_vec.y = y;
        }

        void set_z(const VectorType z) {
            inner_vec.z = z;
        }

        Vector<VectorType> operator+(const Vector<VectorType>& vector) {
            this->inner_vec += vector.inner_vec;
        }

        Vector<VectorType> operator-(const Vector<VectorType>& vector) {
            this->inner_vec -= vector.inner_vec;
        }

        void translate(const Vector<VectorType>& vector) {
            this += vector;
        }

        void translate(const VectorType& x, const VectorType& y, const VectorType& z) {
            this->inner_vec.x += x;
            this->inner_vec.y += y;
            this->inner_vec.z += z;
        }

        void translate_x(const VectorType x) {
            this->inner_vec.x += x;
        }

        void translate_y(const VectorType y) {
            this->inner_vec.y += y;
        }

        void translate_z(const VectorType z) {
            this->inner_vec.z += z;
        }

        static glm::mat4 rotation_matrix(float degrees, glm::vec3 rotation_axis) {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, glm::radians(degrees), rotation_axis);
            return trans;
        }

        static glm::mat4 rotation_x_matrix(float degrees) {
            return rotation_matrix(degrees, glm::vec3(1.f, 0.f, 0.f));
        }

        static glm::mat4 rotation_y_matrix(float degrees) {
            return rotation_matrix(degrees, glm::vec3(0.f, 1.f, 0.f));
        }

        static glm::mat4 rotation_z_matrix(float degrees) {
            return rotation_matrix(degrees, glm::vec3(0.f, 0.f, 1.f));
        }

        void rotate(float degrees, glm::vec3 rotation_axis) {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, glm::radians(degrees), rotation_axis);
            this->inner_vec = trans * this->inner_vec;
        }

        void rotate_x(float degrees) {
            rotate(degrees, glm::vec3(1.f, 0.f, 0.f));
        }

        void rotate_y(float degrees) {
            rotate(degrees, glm::vec3(0.f, 1.f, 0.f));
        }

        void rotate_z(float degrees) {
            rotate(degrees, glm::vec3(0.f, 0.f, 1.f));
        }

        void scale(float scalar) {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::scale(trans, glm::vec3(scalar, scalar, scalar));
            this->inner_vec = trans * this->inner_vec;
        }
    };

    typedef Vector<float> Vectorf;
}