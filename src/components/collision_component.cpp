//
// Created by SeanOMik on 7/6/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "components/collision_component.h"
#include "components/ssma_component.h"
#include "entity.h"

#include <SFML/Graphics/Rect.hpp>
#include <ostream>

simpleengine::CollisionComponent::CollisionComponent(Entity& owning_entity, sf::Transformable &transformable, float offset_x, float offset_y,
        float width, float height) : simpleengine::CollisionComponent::CollisionComponent(owning_entity, transformable, sf::RectangleShape(), offset_x, offset_y) {

    hitbox.setPosition(transformable.getPosition().x + offset_x, transformable.getPosition().y + offset_y);
    hitbox.setSize(sf::Vector2f(width, height));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineThickness(1);
    hitbox.setOutlineColor(sf::Color::Red);
}

simpleengine::CollisionComponent::CollisionComponent(Entity& owning_entity, sf::Transformable& transformable, sf::Vector2f offset, sf::Vector2f size)
    : simpleengine::CollisionComponent::CollisionComponent(owning_entity, transformable, offset.x, offset.y, size.x, size.y) {
}

simpleengine::CollisionComponent::CollisionComponent(Entity& owning_entity, sf::Transformable& transformable, const sf::RectangleShape& hit,
        float offset_x, float offset_y) : Component(owning_entity), transformable(transformable), hitbox(hit), offset_x(offset_x), offset_y(offset_y) {

    // SSMA Component set's the sprites texture offset so we need to set
    // the offset to the origin if the entity has the SSMA component.
    if (auto ssma_comp = owning_entity.GetComponent<simpleengine::SideScrollerMovementAnimationComponent>(); ssma_comp) {
        offset_x += ssma_comp->GetAnimationComponent().GetSprite().getGlobalBounds().width * -0.5f;
    }
}

simpleengine::CollisionComponent::CollisionComponent(Entity& owning_entity, sf::Transformable& transformable, const sf::RectangleShape& hit,
        sf::Vector2f offset) : simpleengine::CollisionComponent::CollisionComponent(owning_entity, transformable, hit, offset.x, offset.y) {

}

sf::RectangleShape &simpleengine::CollisionComponent::GetHitbox() {
    return hitbox;
}

sf::FloatRect CreateBounds(sf::RectangleShape rect) {
    return sf::FloatRect(rect.getPosition(), rect.getSize());
}

bool simpleengine::CollisionComponent::DoesIntersect(const sf::FloatRect &rect) const {
    return CreateBounds(hitbox).intersects(rect);
}

bool simpleengine::CollisionComponent::DoesIntersect(const sf::RectangleShape &rect) const {
    return CreateBounds(hitbox).intersects(CreateBounds(rect));
}

bool simpleengine::CollisionComponent::DoesIntersect(Entity& entity) const {
    std::shared_ptr<simpleengine::CollisionComponent> other_collider = entity.GetComponent<simpleengine::CollisionComponent>();
    if (other_collider) {
        return other_collider->DoesIntersect(hitbox);
    }

    return false;
}

void simpleengine::CollisionComponent::Update(const float &delta_time) {
    hitbox.setPosition(owning_entity.GetTransformable().getPosition().x + offset_x, owning_entity.GetTransformable().getPosition().y + offset_y);
}

void simpleengine::CollisionComponent::Render(sf::RenderTarget *target) {
    //target->draw(hitbox);
}

void simpleengine::CollisionComponent::SetOnCollide(std::function<void(std::shared_ptr<Entity>)> func) {
    collide_function = func;
}

void simpleengine::CollisionComponent::OnCollide(std::shared_ptr<Entity> collided_entity) {
    if (collide_function) {
        collide_function(collided_entity);
    }
}