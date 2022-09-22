#pragma once

#include "component.h"
#include "../../gfx/model.h"

#include <iostream>
#include <vector>

namespace simpleengine {
    /**
     * @brief A Model is a object that will be shown on the screen by a renderer.
     * 
     */
    class ModelComponent : public simpleengine::Component {
    public:
        gfx::Model model;

        ModelComponent(gfx::Model model) : model(model) {

        }

        ModelComponent(std::string model_file_path) : model(model_file_path) {

        }

        virtual void update(const float& delta_time) override {
            
        }
    };
}