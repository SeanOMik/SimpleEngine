//
// Created by SeanOMik on 7/3/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "entity.h"
#include "component.h"

void simpleengine::Entity::Render(sf::RenderTarget *target) {
    RenderComponents(target);
}

void simpleengine::Entity::Update(const float &delta_time) {
    UpdateComponents(delta_time);
}

void simpleengine::Entity::UpdateComponents(const float& delta_time) {
    for (std::unique_ptr<Component>& component : components) {
        component->Update(delta_time);

        if (component->IsGettingDestroyed()) {
            components.erase(std::remove(components.begin(), components.end(), component));
        }
    }
}

void simpleengine::Entity::RenderComponents(sf::RenderTarget* target) {
    for (std::unique_ptr<Component>& component : components) {
        component->Render(target);
    }
}

void simpleengine::Entity::AddComponent(std::unique_ptr<Component> component) {
    components.push_back(std::move(component));
}

void simpleengine::Entity::Destroying() {
    for (std::unique_ptr<Component>& component : components) {
        component->DestroyLater();
    }
}

void simpleengine::Entity::DestroyLater() {
    destroying = true;
}

const bool &simpleengine::Entity::IsGettingDestroyed() const {
    return destroying;
}
