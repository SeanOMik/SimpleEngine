//
// Created by SeanOMik on 7/6/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "components/hitbox_component.h"
#include "components/ssma_component.h"
#include "entity.h"

#include <iostream>

simpleengine::HitboxComponent::HitboxComponent(Entity& owning_entity, sf::Sprite &sprite, float sprite_offset_x, float sprite_offset_y,
        float width, float height) : Component(owning_entity), sprite(sprite), offset_x(sprite_offset_x), offset_y(sprite_offset_y) {

    hitbox.setPosition(sprite.getPosition().x + sprite_offset_x, sprite.getPosition().y + sprite_offset_y);
    hitbox.setSize(sf::Vector2f(width, height));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineThickness(1);
    hitbox.setOutlineColor(sf::Color::Red);

    // SSMA Component set's the sprites texture offset so we need to set
    // the offset to the origin if the entity has the SSMA component.
    if (owning_entity.HasComponent<simpleengine::SideScrollerMovementAnimationComponent>()) {
        offset_x += sprite.getGlobalBounds().width * -0.5f;
    }
}

bool simpleengine::HitboxComponent::DoesIntersect(const sf::FloatRect &rect) {
    return hitbox.getGlobalBounds().intersects(rect);
}

void simpleengine::HitboxComponent::Update(const float &delta_time) {
    hitbox.setPosition(sprite.getPosition().x + offset_x, sprite.getPosition().y + offset_y);
}

void simpleengine::HitboxComponent::Render(sf::RenderTarget *target) {
    target->draw(hitbox);
}
