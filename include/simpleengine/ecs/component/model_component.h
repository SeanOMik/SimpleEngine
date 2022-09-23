#pragma once

#include "../../gfx/model.h"

#include <iostream>
#include <vector>

namespace simpleengine {
    /**
     * @brief A component that contains a Model that will be rendered.
     * 
     */
    class ModelComponent {
    public:
        gfx::Model model;

        ModelComponent(gfx::Model model) : model(model) {

        }

        ModelComponent(std::string model_file_path) : model(model_file_path) {

        }
    };
}