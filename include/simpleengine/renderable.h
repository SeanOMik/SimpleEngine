#pragma once

#include "event/event.h"

#include <memory>

#include <GLFW/glfw3.h>

namespace simpleengine {
    class Renderable : public simpleengine::Event {
    private:
        using Super = simpleengine::Event;
    public:
        Renderable() = default;
        virtual ~Renderable() = default;

        virtual void render(GLFWwindow* target) = 0;
    };
}