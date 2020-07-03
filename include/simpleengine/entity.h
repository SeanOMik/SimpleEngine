//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_ENTITY_H
#define SIMPLEENGINE_ENTITY_H

#include <SFML/Graphics.hpp>

#include <stack>

namespace simpleengine {
    class Component;
    class Game;
    class Event;
    class Entity {
        friend class Game;
        friend class Event;
    public:
        Entity() = default;
        virtual ~Entity() = default;

        virtual void Move(const float& delta_time, const float& x, const float& y) {};
        virtual void Update(const float& delta_time) = 0;
        virtual void Render(sf::RenderTarget* target) = 0;
        virtual void Destroying() {}; // Called when the entity is about to be destroyed.

        void DestroyEntity() {
            destroying = true;
        }

        const bool& IsGettingDestroyed() const {
            return destroying;
        }

        // If your event does not extend from EntityEvent, you will need to execute this yourself inside Event::Update.
        void UpdateComponents(const float& delta_time);

        void AddComponent(Component* component);
    private:
        // This is ran from class `Game` and `Event`. It runs the `UpdateComponents` method and then the `Update` method.
        void UpdateEntity(const float& delta_time) {
            UpdateComponents(delta_time);
        }

        std::vector<Component*> components;
        bool destroying = false;
    };
}

#endif //GAMEENGINE_ENTITY_H
