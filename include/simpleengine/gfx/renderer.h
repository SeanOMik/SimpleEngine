#pragma once

#include "texture.h"
#include "shader.h"
//#include "renderable.h"
#include "model.h"

#include <vector>

namespace simpleengine::gfx {
    class Renderer : public simpleengine::Renderable {
    private:
        GLFWwindow* window;
    public:
        class RenderingModel {
        public:
            RenderingModel(std::shared_ptr<simpleengine::gfx::Model> model, simpleengine::gfx::Texture texture, gfx::VBO ebo,
                gfx::VBO vbo, gfx::VAO vao) : model(model), texture(texture), ebo(ebo), vbo(vbo), vao(vao) {

            }

            std::shared_ptr<simpleengine::gfx::Model> model;
            simpleengine::gfx::Texture texture;

            gfx::VBO ebo;
            gfx::VBO vbo;
            gfx::VAO vao;
        };

        gfx::Shader shader;
        std::vector<RenderingModel> rendering_models;

        Renderer(GLFWwindow* window, gfx::Shader shader);
        Renderer(GLFWwindow* window, GLuint shader_program);
        
        virtual void add_model(simpleengine::gfx::Texture texture, std::shared_ptr<simpleengine::gfx::Model> model);
        virtual void remove_model(std::shared_ptr<simpleengine::gfx::Model> model);

        virtual void initialize();

        virtual void update(const float& delta_time) override {
        
        }
        
        virtual void render(GLFWwindow* target) override;
    };
}