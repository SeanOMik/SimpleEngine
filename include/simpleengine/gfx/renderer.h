#pragma once

#include "../ecs/entity.h"
#include "material.h"
#include "texture.h"
#include "shader.h"
//#include "renderable.h"
#include "model.h"

#include <unordered_map>
#include <vector>

namespace simpleengine::gfx {
    class Renderer : public simpleengine::Renderable {
    private:
        GLFWwindow* window;
    public:
        class RenderingBuffers {
        public:
            gfx::Model& model;
            gfx::VBO ebo;
            gfx::VBO vbo;
            gfx::VAO vao;

            RenderingBuffers(gfx::Model& model, gfx::VBO ebo, gfx::VBO vbo, gfx::VAO vao) : model(model), ebo(ebo), vbo(vbo), vao(vao) {
                
            }

            /* std::vector<LitVertex>& vertices;
            std::vector<GLuint>& indicies; */
            /// If these buffers were rendered last update.
            //bool rendered;
        };

        class RenderingModel {
        public:
            std::shared_ptr<simpleengine::Entity> entity;
            std::unordered_map<uint32_t, RenderingBuffers> rendering_buffers;

            RenderingModel(std::shared_ptr<simpleengine::Entity> entity) : entity(entity) {

            }

            /**
             * @brief Create and delete buffers for new and old components in entity.
             * 
             */
            void update_buffers();

            /**
             * @brief Destroy the buffers
             * 
             */
            void destroy_buffers();
        };

        gfx::Shader shader;
        std::unordered_map<uint32_t, RenderingModel> rendering_models;

        Renderer(GLFWwindow* window, gfx::Shader shader);
        Renderer(GLFWwindow* window, GLuint shader_program);
        
        virtual void submit_entity(std::shared_ptr<simpleengine::Entity> entity);
        virtual bool withdraw_entity(std::shared_ptr<simpleengine::Entity> entity);

        virtual void initialize();
        virtual void destroy();

        virtual void update(const float& delta_time) override;
        
        virtual void render(GLFWwindow* target) override;
    };
}