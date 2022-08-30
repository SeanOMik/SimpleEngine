#pragma once

#include "../../gfx/model.h"
#include "../../event/event.h"

#include <iostream>
#include <vector>

namespace simpleengine {
    /**
     * @brief A Model is a object that will be shown on the screen by a renderer.
     * 
     */
    class Component : public simpleengine::Event {
    private:
        static uint32_t incrementing_handle;
        uint32_t handle;
    public:
        Component() {
            handle = incrementing_handle++;
        }

        uint32_t get_handle() {
            return handle;
        }

        virtual void update(const float& delta_time) override {
            std::cout << "Component update" << std::endl;
        }
    };
}