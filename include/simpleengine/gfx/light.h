#pragma once

#include "shader.h"
#include "../renderable.h"

#include <glm/glm.hpp>

namespace simpleengine::gfx {
    class Light : public simpleengine::Renderable {
    public:
        gfx::Shader shader;
        glm::vec3 position;
        glm::vec3 color;

        Light(gfx::Shader shader, glm::vec3 position, glm::vec3 color) : shader(shader), position(position), color(color) {

        }

        virtual void update(const float& delta_time) override {
            shader.use();
            shader.set_uniform_float_vec3("light_position", position, false);
            shader.set_uniform_float_vec3("light_color", color, false);
            shader.unuse();
        }

        virtual void render(GLFWwindow* target) override {

        }
    };
}