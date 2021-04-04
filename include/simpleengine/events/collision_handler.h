//
// Created by SeanOMik on 3/1/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_COLLISION_HANDLER_H
#define SIMPLEENGINE_COLLISION_HANDLER_H

#include "../event.h"
#include "../entity.h"

namespace simpleengine {
    class Game;
    class CollisionHandler : public Event {
    public:
        explicit CollisionHandler(simpleengine::Game& game) : simpleengine::Event(nullptr), game(game) {

        }

        void Update(const float& delta_time) override;
        void Render(sf::RenderTarget* target) override { }

        void UpdateHandledEntities();
    private:
        std::vector<std::shared_ptr<Entity>> handled_entities;
        simpleengine::Game& game;
    };
}

#endif //SIMPLEENGINE_COLLISION_HANDLER_H
