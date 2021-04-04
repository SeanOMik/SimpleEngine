//
// Created by SeanOMik on 3/1/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "events/collision_handler.h"
#include "components/collision_component.h"
#include "entity.h"
#include "events/entity_event.h"
#include "game.h"

#include <algorithm>

void simpleengine::CollisionHandler::Update(const float& delta_time) {
    // Update list of handled entities.
    handled_entities.erase(std::remove_if(handled_entities.begin(), handled_entities.end(),
        [](std::shared_ptr<simpleengine::Entity> ptr) { return ptr == nullptr; }), handled_entities.end());

    for (simpleengine::Event* event : game.events) {
        auto* entity_event = dynamic_cast<simpleengine::EntityEvent*>(event);
        
        if (entity_event) {
            std::shared_ptr<simpleengine::Entity> entity = entity_event->GetEntity();
            if (std::count(handled_entities.begin(), handled_entities.end(), entity) == 0) {
                handled_entities.push_back(entity);
            }
        }
    }

    // Check for collisions in all entities.
    for (std::shared_ptr<Entity> entity : handled_entities) {
        std::shared_ptr<CollisionComponent> entity_col = entity->GetComponent<simpleengine::CollisionComponent>();

        if (entity_col) {
            for (std::shared_ptr<Entity> two : handled_entities) {
                if (two == entity) continue;

                std::shared_ptr<CollisionComponent> two_col = two->GetComponent<simpleengine::CollisionComponent>();
                if (two_col) {
                    if (entity_col->DoesIntersect(two_col->GetHitbox())) {
                        entity_col->OnCollide(two);
                    }
                }
            }
        }
    }
}