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
            shader.set_uniform_float_vec3("u_light_position", position);
            shader.set_uniform_float_vec3("u_light_color", color);
            shader.unuse();
        }

        virtual void render(const float& interpolate_alpha, const float& frame_time) override {

        }
    };
}