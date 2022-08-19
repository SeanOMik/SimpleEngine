#pragma once

#include "gfx/model.h"
#include "event/event.h"

#include <iostream>
#include <vector>

namespace simpleengine {
    /**
     * @brief A Model is a object that will be shown on the screen by a renderer.
     * 
     */
    class Component : public simpleengine::Event {
    public:
        Component() = default;

        virtual void update(const float& delta_time) override {
            std::cout << "Component update" << std::endl;
        }

        virtual std::vector<std::shared_ptr<gfx::Model>> get_renderable_models() {
            std::cout << "Got renderables" << std::endl;
            return {};
        }
    };
}