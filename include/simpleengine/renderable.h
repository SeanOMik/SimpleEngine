#pragma once
 
#include "event/event.h"

#include <GLFW/glfw3.h>

#include <memory>

namespace simpleengine {
    class Renderable : public simpleengine::Event {
    private:
        using Super = simpleengine::Event;
    public:
        Renderable() = default;
        virtual ~Renderable() = default;

        virtual void render(const float& interpolate_alpha, const float& frame_time) = 0;
    };
}