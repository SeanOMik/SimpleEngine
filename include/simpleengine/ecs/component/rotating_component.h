#pragma once

#include <glm/glm.hpp>

namespace simpleengine::ecs {
    /**
     * @brief A component that will rotate the transform every frame.
     * 
     */
    class RotatingComponent {
    public:
        float rate;
        glm::vec3 rotation_axis;

        RotatingComponent() : rate(10.f), rotation_axis(glm::vec3(0, 1, 0)) {

        }

        RotatingComponent(float rate) : rate(rate), rotation_axis(glm::vec3(0, 1, 0)) {

        }

        RotatingComponent(glm::vec3 rotation_axis) : rotation_axis(rotation_axis) {

        }

        RotatingComponent(float rate, glm::vec3 rotation_axis) : rate(rate), rotation_axis(rotation_axis) {

        }
    };
}