#pragma once

#include "component.h"
#include "../../gfx/mesh.h"
#include "../../gfx/material.h"

#include <iostream>
#include <vector>

namespace simpleengine {
    /**
     * @brief A Model is a object that will be shown on the screen by a renderer.
     * 
     */
    class MeshComponent : public simpleengine::Component {
    public:
        gfx::Mesh model;
        //gfx::Material material;

        MeshComponent(gfx::Mesh model) : model(model) {

        }

        MeshComponent(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, gfx::Material material,
                bool calculate_normals = false): model(vertices, indicies, material) {

            if (calculate_normals) {
                model.calculate_normals();
            }
        }

        MeshComponent(std::vector<LitVertex> vertices, std::vector<GLuint> indicies = std::vector<GLuint>(),
                std::optional<gfx::Material> material = std::nullopt, bool calculate_normals = false) :
                model(vertices, indicies, material) {

            if (calculate_normals) {
                model.calculate_normals();
            }
        }

        virtual void update(const float& delta_time) override {
            
        }
    };
}