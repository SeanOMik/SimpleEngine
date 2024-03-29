//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_COMPONENT_H
#define SIMPLEENGINE_COMPONENT_H

#include "destructable.h"

#include <SFML/Graphics.hpp>

#include <memory>

namespace simpleengine {
    class Entity;
    class Component : public simpleengine::Destructable {
    public:
        explicit Component(Entity& owning_entity) : owning_entity(owning_entity) {

        }
        virtual ~Component() = default;

        virtual void Update(const float& delta_time) = 0;
        virtual void Render(sf::RenderTarget* target) {}; // Most components won't need to be rendered.
    protected:
        Entity& owning_entity;
        bool destroying = false;
    };
}

#endif //SIMPLEENGINE_COMPONENT_H
