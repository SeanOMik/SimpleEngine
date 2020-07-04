//
// Created by SeanOMik on 7/3/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_ENTITY_EVENT_H
#define SIMPLEENGINE_ENTITY_EVENT_H

#include "../event.h"
#include "../entity.h"

namespace simpleengine {
    class EntityEvent : public Event {
    public:
        explicit EntityEvent(sf::RenderWindow* window, std::unique_ptr<Entity> entity) : simpleengine::Event(window), entity(std::move(entity)) {

        }

        void CheckForQuit() override {

        }

        void Update(const float& delta_time) override {
            entity->Update(delta_time);
            entity->UpdateComponents(delta_time);

            if (entity->IsGettingDestroyed()) {
                quit = true;
            }
        }

        void Render(sf::RenderTarget* target) override {
            entity->Render(target);
        }
    private:
        std::unique_ptr<Entity> entity;
    };
}

#endif //SIMPLEENGINE_ENTITY_EVENT_H
