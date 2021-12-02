#pragma once

#include "../destructable.h"

#include <memory>

#include <GLFW/glfw3.h>

namespace simpleengine {
    class Event : public simpleengine::Destructable {
    public:
        explicit Event(GLFWwindow* window = nullptr) : window(window) {}
        virtual ~Event() = default;
        
        virtual void update(const float& delta_time) = 0;
        virtual void render(GLFWwindow* target) = 0;
    protected:
        GLFWwindow* window;
    };
}