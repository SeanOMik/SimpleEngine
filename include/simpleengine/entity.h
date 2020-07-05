//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_ENTITY_H
#define SIMPLEENGINE_ENTITY_H

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

namespace simpleengine {
    class Component;
    class Game;
    class Event;

    // @TODO Create a Destructible class that replaces Entity::Destroying, Entity::DestroyLater, and Entity::IsGettingDestroyed.
    class Entity {
        friend class Game;
        friend class Event;
    public:
        Entity() = default;
        virtual ~Entity() = default;
        Entity(const Entity& entity) = delete;

        virtual void Move(const float& delta_time, const float& dir_x, const float& dir_y) {};
        virtual void Move(const float& delta_time, const sf::Vector2f& offset) {};
        virtual void Move(const sf::Vector2f& offset) {};

        virtual void Render(sf::RenderTarget* target) = 0;
        virtual void Update(const float& delta_time) {
            UpdateComponents(delta_time);
        };

        // Called when the entity is about to be destroyed.
        // Make sure to call this in your extending Entity.
        virtual void Destroying();
        void DestroyLater(); // In most cases, this will be ran next EntityEvent::Update()
        const bool& IsGettingDestroyed() const;

        void UpdateComponents(const float& delta_time);
        void AddComponent(std::unique_ptr<Component> component);

        std::shared_ptr<Entity> GetShared() {
            return shared_from_this();
        }
    protected:
        std::vector<std::unique_ptr<Component>> components;
        bool destroying = false;
    };
}

#endif //GAMEENGINE_ENTITY_H
