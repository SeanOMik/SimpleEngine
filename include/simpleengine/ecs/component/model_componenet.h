#pragma once

#include "component.h"
#include "../../gfx/model.h"
#include "../../gfx/material.h"

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
        //gfx::Material material;

        ModelComponent(gfx::Model model) : model(model) {

        }

        ModelComponent(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, gfx::Material material,
                bool calculate_normals = false): model(vertices, indicies, material) {

            if (calculate_normals) {
                model.calculate_normals();
            }
        }

        ModelComponent(std::vector<LitVertex> vertices, std::vector<GLuint> indicies = std::vector<GLuint>(),
                std::optional<gfx::Material> material = std::nullopt, bool calculate_normals = false) :
                model(vertices, indicies, material) {

            if (calculate_normals) {
                model.calculate_normals();
            }
        }

        virtual void update(const float& delta_time) override {
            std::cout << "Model Component update" << std::endl;
        }
    };
}