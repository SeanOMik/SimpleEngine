#pragma once

#include "../../gfx/mesh.h"
#include "../../gfx/material.h"

#include <iostream>
#include <vector>

namespace simpleengine::ecs {
    /**
     * @brief A component that contains a Mesh that will be rendered.
     * 
     */
    class MeshComponent {
    public:
        gfx::Mesh mesh;

        MeshComponent(gfx::Mesh mesh) : mesh(mesh) {

        }

        MeshComponent(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, gfx::Material material,
                bool calculate_normals = false): mesh(vertices, indicies, material) {

            if (calculate_normals) {
                mesh.calculate_normals();
            }
        }

        MeshComponent(std::vector<LitVertex> vertices, std::vector<GLuint> indicies = std::vector<GLuint>(),
                std::optional<gfx::Material> material = std::nullopt, bool calculate_normals = false) :
                mesh(vertices, indicies, material) {

            if (calculate_normals) {
                mesh.calculate_normals();
            }
        }
    };
}