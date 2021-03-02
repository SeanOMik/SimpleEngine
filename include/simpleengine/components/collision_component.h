//
// Created by SeanOMik on 7/6/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_COLLISION_COMPONENT_H
#define SIMPLEENGINE_COLLISION_COMPONENT_H

#include "../component.h"
#include "../entity.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <functional>

namespace simpleengine {
    class CollisionComponent : public Component {
    private:
        sf::Transformable& transformable;
        sf::RectangleShape hitbox;

        float offset_x;
        float offset_y;
        std::function<void(std::shared_ptr<Entity>)> collide_function;
    public:
        CollisionComponent(Entity& owning_entity, sf::Transformable& transformable, float offset_x, float offset_y, float width, float height);
        CollisionComponent(Entity& owning_entity, sf::Transformable& transformable, sf::Vector2f offset, sf::Vector2f size);

        CollisionComponent(Entity& owning_entity, sf::Transformable& transformable, const sf::RectangleShape& hitbox, float offset_x, float offset_y);
        CollisionComponent(Entity& owning_entity, sf::Transformable& transformable, const sf::RectangleShape& hitbox, sf::Vector2f offset);

        sf::RectangleShape& GetHitbox();

        virtual bool DoesIntersect(const sf::FloatRect& rect) const;
        virtual bool DoesIntersect(const sf::RectangleShape& rect) const;
        virtual bool DoesIntersect(Entity& entity) const;

        void Update(const float &delta_time) override;
        void Render(sf::RenderTarget *target) override;

        virtual void SetOnCollide(std::function<void(std::shared_ptr<Entity>)> func);
        virtual void OnCollide(std::shared_ptr<Entity> collided_entity);
    };
}

#endif //SIMPLEENGINE_COLLISION_COMPONENT_H
