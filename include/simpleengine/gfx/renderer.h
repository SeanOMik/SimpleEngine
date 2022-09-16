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

        class RenderingModel {
        public:
            std::shared_ptr<simpleengine::Entity> entity;
            std::unordered_map<uint32_t, gfx::Model&> component_models;

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

        std::unordered_map<uint32_t, RenderingModel> rendering_models;
        gfx::Shader shader;

        Renderer(GLFWwindow* window, gfx::Shader shader);
        Renderer(GLFWwindow* window, GLuint shader_program);

        void enable_debug();

        virtual void submit_entity(std::shared_ptr<simpleengine::Entity> entity);
        virtual bool withdraw_entity(std::shared_ptr<simpleengine::Entity> entity);

        virtual void initialize();
        virtual void destroy() override;

        virtual void update(const float& delta_time) override;
        
        virtual void render() override;
    };
}