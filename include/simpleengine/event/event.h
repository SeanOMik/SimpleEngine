//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_EVENT_H
#define SIMPLEENGINE_EVENT_H

#include "../destructable.h"

#include <memory>

#include <GLFW/glfw3.h>

namespace simpleengine {
    class Event : public simpleengine::Destructable {
    public:
        explicit Event(std::shared_ptr<GLFWwindow> window = nullptr) : window(window) {}
        virtual ~Event() = default;
        
        virtual void update(const float& delta_time) = 0;
        virtual void render(std::shared_ptr<GLFWwindow> target) = 0;
    protected:
        std::shared_ptr<GLFWwindow> window;
    };
}

#endif //GAMEENGINE_EVENT_H