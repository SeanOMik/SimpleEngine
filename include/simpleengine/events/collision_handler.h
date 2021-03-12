//
// Created by SeanOMik on 3/1/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_COLLISION_HANDLER_H
#define SIMPLEENGINE_COLLISION_HANDLER_H

#include "../event.h"
#include "../entity.h"

namespace simpleengine {
    class CollisionHandler : public Event {
    public:
        explicit CollisionHandler(sf::RenderWindow* window, std::vector<std::shared_ptr<Entity>> entities) : simpleengine::Event(window), entities(entities) {

        }

        void Update(const float& delta_time) override;
        void CheckForQuit() override { }
        void Render(sf::RenderTarget* target) override { }
    private:
        std::vector<std::shared_ptr<Entity>> entities;
    };
}

#endif //SIMPLEENGINE_COLLISION_HANDLER_H
