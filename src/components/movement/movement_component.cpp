//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "components/movement/movement_component.h"
#include "entity.h"

simpleengine::MovementComponent::MovementComponent(simpleengine::Entity *owning_entity, float movement_speed) :
    Component(owning_entity), movement_speed(movement_speed) {

}

void simpleengine::MovementComponent::Update(const float &delta_time) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        owning_entity->Move(delta_time, -1, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        owning_entity->Move(delta_time, 0, -1);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        owning_entity->Move(delta_time, 1, 0);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        owning_entity->Move(delta_time, 0, 1);
    }
}
