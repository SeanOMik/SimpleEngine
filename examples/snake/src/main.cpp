//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include <iostream>

#include <simpleengine/game.h>
#include <simpleengine/event.h>
#include <simpleengine/entity.h>
#include <simpleengine/components/movement/movement_component.h>
#include <simpleengine/events/entity_event.h>

class SnakeMovementComponent : public simpleengine::Component {
public:
    explicit SnakeMovementComponent(simpleengine::Entity* owning_entity, float movement_speed) : simpleengine::Component(owning_entity), movement_speed(movement_speed) {

    }

    void Update(const float &delta_time) override {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            movement_direction.x = -1;
            movement_direction.y = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            movement_direction.x = 0;
            movement_direction.y = -1;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movement_direction.x = 1;
            movement_direction.y = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movement_direction.x = 0;
            movement_direction.y = 1;
        }

        owning_entity->Move(delta_time, movement_direction.x, movement_direction.y);
    }
private:
    float movement_speed;
    sf::Vector2f movement_direction;
};

class SnakePlayerEntity : public simpleengine::Entity {
private:
    sf::RectangleShape shape;
    float movement_speed = 100;
    sf::Vector2u window_size;
    sf::Vector2f loc;
public:
    explicit SnakePlayerEntity(sf::Vector2u window_size) : window_size(window_size) {
        shape = sf::RectangleShape(sf::Vector2f(15, 15));
        shape.setFillColor(sf::Color::White);

        this->AddComponent(new SnakeMovementComponent(this, movement_speed));
        loc = sf::Vector2f(0, 0);
    }

    void Move(const float& delta_time, const float& x, const float& y) override {
        shape.move(x * movement_speed * delta_time, y * movement_speed * delta_time);
    }

    void Update(const float& delta_time) override {
        UpdateComponents(delta_time);

        if (loc.x >= window_size.x) {
            DestroyEntity();
        }

        if (loc.y >= window_size.y) {
            DestroyEntity();
        }
    }

    void Render(sf::RenderTarget* target) override {
        target->draw(shape);
    }
};

int main(int argc, char *argv[]) {
    simpleengine::Game game(500, 500, "First Example");
    game.AddEvent(new simpleengine::EntityEvent(game.GetWindow(), new SnakePlayerEntity(game.GetWindow()->getSize())));

    return game.Run();
}