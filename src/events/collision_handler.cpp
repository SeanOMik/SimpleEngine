//
// Created by SeanOMik on 3/1/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "events/collision_handler.h"
#include "components/collision_component.h"

void simpleengine::CollisionHandler::Update(const float& delta_time) {
    for (std::shared_ptr<Entity> entity : entities) {
        std::shared_ptr<CollisionComponent> entity_col = entity->GetComponent<simpleengine::CollisionComponent>();

        if (entity_col) {
            for (std::shared_ptr<Entity> two : entities) {
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