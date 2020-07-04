//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_MOVEMENT_COMPONENT_H
#define SIMPLEENGINE_MOVEMENT_COMPONENT_H

#include "../../component.h"

#include <SFML/Graphics.hpp>

namespace simpleengine {
    class Entity;

    class MovementComponent : public Component {
    public:
        MovementComponent(Entity& owning_entity, float max_velocity, float acceleration = 7, float deceleration = 2.5);

        void Move(const float &delta_time, const float& dir_x, const float& dir_y);
        void Update(const float &delta_time) override;
    private:
        float max_velocity;
        sf::Vector2f velocity;
        float acceleration;
        float deceleration;
    };
}

#endif //SIMPLEENGINE_MOVEMENT_COMPONENT_H
