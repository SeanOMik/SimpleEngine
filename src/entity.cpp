//
// Created by SeanOMik on 7/3/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "entity.h"
#include "component.h"

simpleengine::Entity::Entity(sf::Transformable &transformable) : transformable(transformable) {

}

void simpleengine::Entity::Move(const float &delta_time, const float &dir_x, const float &dir_y) {
    transformable.setPosition(dir_x * delta_time, dir_y * delta_time);
}

void simpleengine::Entity::Move(const float &delta_time, const sf::Vector2f &offset) {
    transformable.move(offset * delta_time);
}

void simpleengine::Entity::Move(const sf::Vector2f &offset) {
    transformable.move(offset);
}

void simpleengine::Entity::Render(sf::RenderTarget *target) {
    RenderComponents(target);
}

void simpleengine::Entity::Update(const float &delta_time) {
    UpdateComponents(delta_time);
}

void simpleengine::Entity::UpdateComponents(const float& delta_time) {
    // Update each component and if they are being destroyed, destroy them.
    for (std::vector<std::shared_ptr<Component>>::iterator it = components.begin(); it != components.end(); ) {
        (*it)->Update(delta_time);

        if ((*it)->IsDestroying()) {
            it = components.erase(it);
        } else {
            ++it;
        }
    }
}

void simpleengine::Entity::RenderComponents(sf::RenderTarget* target) {
    for (std::shared_ptr<Component>& component : components) {
        component->Render(target);
    }
}

void simpleengine::Entity::Destroy() {
    destroying = true;
}

const bool& simpleengine::Entity::IsDestroying() const {
    return destroying;
}
