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
     * @brief A Model is a object that will be shown on the screen by a renderer.
     * 
     */
    class Model : public simpleengine::Event, public simpleengine::Transformable {
    public:
        std::optional<Material> material;
        std::vector<LitVertex> vertices;
        std::vector<GLuint> indicies;

        // Buffer objects
        gfx::VBO ebo;
        gfx::VBO vbo;
        gfx::VAO vao;

        Model(std::vector<LitVertex> vertices, std::vector<GLuint> indicies, Material material);
        Model(std::vector<LitVertex> vertices, std::vector<GLuint> indicies = std::vector<GLuint>(), std::optional<Material> material = std::nullopt);
        Model(Material material, std::string filename);
        Model(Material material, std::ifstream file_stream);

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

    private:
        void process_vertex(const std::vector<std::string>& vertex_data, const std::vector<glm::vec2>& in_textures,
            const std::vector<glm::vec3>& in_normals, std::vector<GLuint>& out_indicies, 
            std::vector<glm::vec2>& out_textures, std::vector<glm::vec3>& out_normals);
    };
}