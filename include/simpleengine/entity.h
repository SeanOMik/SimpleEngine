//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_ENTITY_H
#define SIMPLEENGINE_ENTITY_H

#include <SFML/Graphics.hpp>

namespace simpleengine {
    class Entity {
    public:
        Entity() = default;
        virtual ~Entity() = default;

        virtual void Move(const float& delta_time, const float& x, const float& y) {};
        virtual void Update(const float& delta_time) = 0;
        virtual void Render(sf::RenderTarget* target = nullptr) = 0;
    };
}

#endif //GAMEENGINE_ENTITY_H
