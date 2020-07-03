//
// Created by SeanOMik on 7/3/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "entity.h"
#include "component.h"

void simpleengine::Entity::UpdateComponents(const float& delta_time) {
    for (Component* component : components) {
        component->Update(delta_time);
    }
}

void simpleengine::Entity::AddComponent(simpleengine::Component *component) {
    components.emplace_back(component);
}
