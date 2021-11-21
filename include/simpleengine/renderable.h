//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_RENDERABLE_H
#define SIMPLEENGINE_RENDERABLE_H

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

#endif //SIMPLEENGINE_RENDERABLE_H