#pragma once

#include "../../gfx/model.h"
#include "../../gfx/texture.h"
#include "../../gfx/textured_model.h"
#include "../../renderable.h"
#include "../../vertex.h"
#include <vector>

namespace simpleengine::objects_3d {
    class Terrain : public simpleengine::gfx::TexturedModel {
    private:
        /**
         * @brief DO NOT USE!!! Replaced with lit_vertices
         * 
         */
        using gfx::Model::vertices;
    public:
        std::vector<LitVertex> lit_vertices;
        const float size;
        const int vertex_count;

        float x;
        float y;

        gfx::Texture texture;

        Terrain(GLFWwindow* window, gfx::Shader shader, gfx::Texture texture, int grid_x, int grid_y, float size = 800.f, int vertex_count = 128) :
                simpleengine::gfx::TexturedModel(window, shader, { texture }, std::vector<LitVertex>()), x(grid_x * size), y(grid_y * size),
                texture(texture), size(size), vertex_count(vertex_count) {

            generate_terrain();
        }

    protected:
        virtual void setup_vertices() override {
            vao.bind();
            vbo.buffer(lit_vertices.data(), 0, sizeof(LitVertex) * lit_vertices.size());
            ebo.buffer(indicies.data(), 0, indicies.size() * sizeof(GLuint));

            // Enable VAO attributes
            vao.enable_attrib(vbo, 0, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, position));
            //vao.enable_attrib(vbo, 1, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, color));
            vao.enable_attrib(vbo, 1, 2, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, tex_coord));
            vao.enable_attrib(vbo, 2, 3, GL_FLOAT, sizeof(LitVertex), offsetof(LitVertex, normal));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    public:
        void generate_terrain() {
            lit_vertices.clear();
            indicies.clear();

            // Reserve space for vertices and indicies before hand.
            int count = pow(vertex_count, 2);
            lit_vertices.reserve(count);
            indicies.reserve(6 * pow(vertex_count - 1, 2));

            int vertex_index = 0;
            for (int i = 0; i < vertex_count; i++) {
                for (int j = 0; j < vertex_count; j++) {
                    Vectorf pos( (float) j / ((float) vertex_count - 1) * size, 0, (float) i / ((float) vertex_count - 1) * size);
                    glm::vec2 tex( (float) j / (float) vertex_count - 1, (float) i / (float) vertex_count - 1);
                    glm::vec3 normals(0, 1, 0);
                    lit_vertices.emplace_back(pos, tex, normals);

                    vertex_index++;
                }
            }

            int indicies_index = 0;
            for (int i = 0; i < vertex_count; i++) {
                for (int j = 0; j < vertex_count; j++) {
                    int top_left = (i * vertex_count) + j;
                    int top_right = top_left + 1;
                    int bottom_left = ((i + 1) * vertex_count) + j;
                    int bottom_right = bottom_left + 1;

                    indicies.push_back(top_left);
                    indicies.push_back(bottom_left);
                    indicies.push_back(top_right);
                    indicies.push_back(top_right);
                    indicies.push_back(bottom_left);
                    indicies.push_back(bottom_right);
                }
            }

            std::cout << "========= New terrain generated =========" << std::endl;
            std::cout << "    Lit vertices count: " << lit_vertices.size() << std::endl;
            std::cout << "    Indicies count: " << indicies.size() << std::endl;

            setup_vertices();
        }

        virtual void update(const float& delta_time) override {

        }
        
        /* virtual void render(GLFWwindow* target) override {

        } */
    };
}