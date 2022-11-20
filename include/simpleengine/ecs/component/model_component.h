#pragma once

#include "../../gfx/model.h"

#include <iostream>
#include <vector>

namespace simpleengine::ecs {
    /**
     * @brief A component that contains a Model that will be rendered.
     * 
     */
    class ModelComponent {
    public:
        gfx::Model model;

        ModelComponent(gfx::Model model) : model(model) {

        }

        ModelComponent(std::string model_file_path, int model_processing_flags = gfx::ModelProcessingFlags::MdlProcFlag_NONE,
            int assimp_flags = 0) : model(model_file_path, model_processing_flags, assimp_flags) {

        }
    };
}