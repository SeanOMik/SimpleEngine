#pragma once

#include "component.h"
#include "transformable.h"

#include <iterator>
#include <vector>
#include <iostream>

namespace simpleengine {
    /**
     * @brief A Model is a object that will be shown on the screen by a renderer.
     * 
     */
    class Entity : public simpleengine::Event, public simpleengine::Transformable {
    public:
        std::vector<Component> components;

        Entity(std::vector<Component> components = {}) : components(components) {

        }

        virtual void update(const float& delta_time) override {
            std::cout << "Update entity" << std::endl;

            for (auto& component : components) {
                component.update(delta_time);
            }
        }

        virtual std::vector<std::shared_ptr<gfx::Model>> get_renderable_models() {
            std::cout << "Got renderables from entity" << std::endl;
            
            std::vector<std::shared_ptr<gfx::Model>> models;
            for (auto& component : components) {
                std::vector<std::shared_ptr<gfx::Model>> comp_models = component.get_renderable_models();

                // Move comp_models into models
                models.insert(models.end(), std::make_move_iterator(comp_models.begin()),
                    std::make_move_iterator(comp_models.end()));
            }

            return models;
        }
    };
}