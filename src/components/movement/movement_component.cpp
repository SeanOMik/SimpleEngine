//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "components/movement/movement_component.h"
#include "entity.h"

#include <algorithm>

simpleengine::MovementComponent::MovementComponent(Entity& owning_entity, float max_velocity, float acceleration, float deceleration) :
    Component(owning_entity), max_velocity(max_velocity), acceleration(acceleration), deceleration(deceleration) {

}

void simpleengine::MovementComponent::Move(const float& delta_time, const float& dir_x, const float& dir_y) {
    // Acceleration
    velocity.x += acceleration * dir_x;
    // Limit the horizontal velocity
    if (velocity.x > 0) { // Right
        velocity.x = std::min(velocity.x, max_velocity);
    } else if (velocity.x < 0) { // Left
        velocity.x = std::max(velocity.x, -max_velocity);
    }

    velocity.y += acceleration * dir_y;
    // Limit the vertical velocity
    if (velocity.y > 0) { // Up
        velocity.y = std::min(velocity.y, max_velocity);
    } else if (velocity.y < 0) { // Down
        velocity.y = std::max(velocity.y, -max_velocity);
    }

    // Limit total velocity
    float y_sq = velocity.y * velocity.y;
    float x_sq = velocity.x * velocity.x;
    float abs_sq = y_sq + x_sq;
    if (abs_sq > max_velocity * max_velocity) {
        float abs = sqrt(abs_sq);
        velocity.x *= max_velocity / abs;
        velocity.y *= max_velocity / abs;
    }

    owning_entity.Move(delta_time, velocity);
}

void simpleengine::MovementComponent::Update(const float& delta_time) {
    sf::Vector2f direction;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction.x = -1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction.y = -1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction.x = 1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction.y = 1;
    }

    //std::cout << "Speed: " << velocity.x << ", " << velocity.y << std::endl;

    // If we're not moving from deceleration or player input, then reset velocity.
    bool no_movement = true;

    if (direction.y != 0 || direction.x == 0) {
        if (velocity.y > 0) {
            if (velocity.y > max_velocity) {
                velocity.y = max_velocity;
            }

            velocity.y -= deceleration;
            if (velocity.y < 0) {
                velocity.y = 0;
            }

            owning_entity.Move(velocity * delta_time);
            no_movement = false;
        } else if (velocity.y < 0) {
            if (velocity.y < -max_velocity) {
                velocity.y = -max_velocity;
            }

            velocity.y += deceleration;
            if (velocity.y > 0) {
                velocity.y = 0;
            }

            owning_entity.Move(velocity * delta_time);
            no_movement = false;
        }
    }

    if (direction.x != 0 || direction.y == 0) {
        if (velocity.x > 0) {
            if (velocity.x > max_velocity) {
                velocity.x = max_velocity;
            }

            velocity.x -= deceleration;
            if (velocity.x < 0) {
                velocity.x = 0;
            }

            owning_entity.Move(velocity * delta_time);
            no_movement = false;
        } else if (velocity.x < 0) {
            if (velocity.x < -max_velocity) {
                velocity.x = -max_velocity;
            }

            velocity.x += deceleration;
            if (velocity.x > 0) {
                velocity.x = 0;
            }

            owning_entity.Move(velocity * delta_time);
            no_movement = false;

        }
    }

    if (direction.x != 0 || direction.y != 0) {
        last_direction = direction;
        Move(delta_time, direction.x, direction.y);
        no_movement = false;
    }

    if (no_movement) {
        velocity = sf::Vector2f();
    }
}

const sf::Vector2f &simpleengine::MovementComponent::GetVelocity() const {
    return velocity;
}

const sf::Vector2f &simpleengine::MovementComponent::GetLastDirection() const {
    return last_direction;
}
