#pragma once

#include "gfx/mesh.h"
#include "entity.h"
#include "event/event.h"
#include "renderable.h"

#include <memory>

#include <GLFW/glfw3.h>
#include <vector>

namespace simpleengine {
    class Scene : public simpleengine::Event {
    public:
        /**
         * @brief A list of entities in this scene.
         * 
         */
        std::vector<std::shared_ptr<Entity>> entities;

        /**
         * @brief Models that don't belong to an entity.
         * 
         */
        std::vector<std::shared_ptr<gfx::Model>> stray_models;

        Scene() = default;

        void add_entity(std::shared_ptr<Entity> entity) {
            entities.push_back(entity);
        }

        void add_stray_model(std::shared_ptr<gfx::Model> stray) {
            stray_models.push_back(stray);
        }

        virtual void update(const float& delta_time) override {
            for (auto& entity : entities) {
                entity->update(delta_time);
            }
        }
    };
}