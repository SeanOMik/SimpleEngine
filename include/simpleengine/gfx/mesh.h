#pragma once

#include "shader.h"
#include "../event/event.h"
#include "vao.h"
#include "vbo.h"
#include "../vertex.h"
#include "../renderable.h"
#include "../transformable.h"
#include "material.h"

#include <optional>
#include <vector>

namespace simpleengine::gfx {
    /**
     * @brief A Mesh is a object that will be shown on the screen by a renderer.
     * 
     */
    class Mesh : public simpleengine::Event, public simpleengine::Transformable {
    public:
        std::optional<Material> material;
        std::vector<LitVertex> vertices;
        std::vector<GLuint> indicies;

        gfx::VBO ebo;
        gfx::VBO vbo;
        gfx::VAO vao;

        Mesh(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, Material material);
        Mesh(std::vector<LitVertex> vertices, std::vector<GLuint> indicies = std::vector<GLuint>(), std::optional<Material> material = std::nullopt);
        Mesh(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, Material material, gfx::VBO ebo, gfx::VBO vbo, gfx::VAO vao);

        virtual void destroy() override;

        virtual void update(const float& delta_time) override;

        glm::vec3 compute_face_normal(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

        /**
         * @brief Calculate the normals of the model.
         * 
         * @note This **will** overwrite the existing normals.
         *
         */
        void calculate_normals();
    };
}