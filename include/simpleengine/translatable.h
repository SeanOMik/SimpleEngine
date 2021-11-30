#pragma once

#include <glm/detail/qualifier.hpp>
#include <glm/glm.hpp>

namespace simpleengine {
    template<typename T = float>
    class Translatable {
    protected:

    public:
        glm::mat<4, 4, T, glm::defaultp> translation_matrix;

        virtual Translatable<T> operator+(const Translatable<T> rhs) = 0;
        virtual Translatable<T> operator-(const Translatable<T> rhs) = 0;

        virtual void translate(const glm::mat<4, 4, T, glm::defaultp>& translate_vec) = 0;
        virtual void translate(const Translatable<T>& translate_vec) = 0;
        virtual void translate(T x, T y, T z) = 0;
        virtual void translate(const T& x, const T& y, const T& z) = 0;

        virtual glm::mat<4, 4, T, glm::defaultp> rotation_matrix(float degrees, glm::vec3 rotation_axis) const = 0;
        virtual glm::mat<4, 4, T, glm::defaultp> rotation_x_matrix(float degrees) const = 0;
        virtual glm::mat<4, 4, T, glm::defaultp> rotation_y_matrix(float degrees) const = 0;
        virtual glm::mat<4, 4, T, glm::defaultp> rotation_z_matrix(float degrees) const = 0;

        virtual void rotate(float degrees, glm::vec3 rotation_axis) = 0;
        virtual void rotate_x(float degrees) = 0;
        virtual void rotate_y(float degrees) = 0;
        virtual void rotate_z(float degrees) = 0;

        virtual void scale(float scalar) = 0;
    };
}