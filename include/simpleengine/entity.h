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
#include <assert.h>

namespace simpleengine {
    class Component;
    class Game;
    class Event;

    // @TODO Create a Destructible class that replaces Entity::Destroying, Entity::DestroyLater, and Entity::IsGettingDestroyed.
    class Entity {
        friend class Game;
        friend class Event;
    public:
        explicit Entity(sf::Sprite& sprite);
        virtual ~Entity() = default;
        Entity(const Entity& entity) = delete;

        virtual void Move(const float& delta_time, const float& dir_x, const float& dir_y);
        virtual void Move(const float& delta_time, const sf::Vector2f& offset);
        virtual void Move(const sf::Vector2f& offset);

        virtual void Render(sf::RenderTarget* target);
        virtual void Update(const float& delta_time);

        // Called when the entity is about to be destroyed.
        // Make sure to call this in your extending Entity.
        virtual void Destroying();
        void DestroyLater(); // In most cases, this will be ran next EntityEvent::Update()
        const bool& IsGettingDestroyed() const;

        template<typename T>
        bool HasComponent() {
            for (std::shared_ptr<Component>& comp : components) {
                if (dynamic_cast<T*>(comp.get())) {
                    return true;
                }
            }

            return false;
        }

        void UpdateComponents(const float& delta_time);
        void RenderComponents(sf::RenderTarget* target);

        template<typename T>
        void AddComponent(std::shared_ptr<T> component) {
            static_assert(std::is_base_of_v<Component, T>, "Component class must derive from simpleengine::Component");

            // Only allow one type of the same component.
            assert(!HasComponent<T>());
            components.push_back(component);
        }

        sf::Sprite& GetSprite();
    protected:
        sf::Sprite& sprite;
        std::vector<std::shared_ptr<Component>> components;
        bool destroying = false;
    };
}

#endif //GAMEENGINE_ENTITY_H
