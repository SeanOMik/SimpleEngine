#pragma once

#include "event/event.h"

#include <memory>

#include <GLFW/glfw3.h>

namespace simpleengine {
    class Renderable : public simpleengine::Event {
    private:
        using super = simpleengine::Event;
    public:
        explicit Renderable(std::shared_ptr<GLFWwindow> window = nullptr) : super(window) {}
        virtual ~Renderable() = default;
    };
}