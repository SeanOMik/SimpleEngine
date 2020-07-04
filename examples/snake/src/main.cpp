//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include <simpleengine/game.h>
#include <simpleengine/event.h>
#include <simpleengine/entity.h>
#include <simpleengine/components/movement/movement_component.h>
#include <simpleengine/events/entity_event.h>

#include <chrono>
#include <iostream>

class SnakeMovementComponent : public simpleengine::Component {
private:
    float movement_speed;
    sf::Vector2f movement_direction;
    std::chrono::high_resolution_clock::time_point last_movement;
public:
    explicit SnakeMovementComponent(simpleengine::Entity& owning_entity, float movement_speed) : simpleengine::Component(owning_entity), movement_speed(movement_speed) {

    }

    void Update(const float &delta_time) override {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            movement_direction.x = -movement_speed;
            movement_direction.y = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            movement_direction.x = 0;
            movement_direction.y = -movement_speed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movement_direction.x = movement_speed;
            movement_direction.y = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movement_direction.x = 0;
            movement_direction.y = movement_speed;
        }

        int duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last_movement).count();

        if (duration >= 115 && (movement_direction.x != 0 || movement_direction.y != 0)) {
            owning_entity.Move(delta_time, movement_direction.x, movement_direction.y);
            last_movement = std::chrono::high_resolution_clock::now();
        }
    }
};

class SnakePlayerEntity : public simpleengine::Entity {
private:
    sf::RectangleShape shape;
    float movement_speed = 15;
    sf::Vector2u window_size;
public:
    explicit SnakePlayerEntity(sf::Vector2u window_size) : window_size(window_size) {
        shape = sf::RectangleShape(sf::Vector2f(15, 15));
        shape.setFillColor(sf::Color::White);

        AddComponent(std::make_unique<SnakeMovementComponent>(*this, movement_speed));
    }

    ~SnakePlayerEntity() override {
        std::cout << "Destroying" << std::endl;

        simpleengine::Entity::~Entity();
    }

    void Move(const float& delta_time, const float& x, const float& y) override {
        /*float move_x = shape.getSize().x * x;
        float move_y = shape.getSize().y * y;*/

        shape.move(x, y);
    }

    void Move(const float& delta_time, const sf::Vector2f& offset) override {
        shape.move(offset * delta_time);
    }

    void Move(const sf::Vector2f& offset) override {
        shape.move(offset);
    }

    void Update(const float& delta_time) override {
        /*UpdateComponents(delta_time);

        if (shape.getPosition().x - shape.getSize().x < 0 || shape.getPosition().x + shape.getSize().x > window_size.x) {
            DestroyLater();
        } else if (shape.getPosition().y - shape.getSize().y < 0 || shape.getPosition().y + shape.getSize().y > window_size.y) {
            DestroyLater();
        }*/
    }

    void Render(sf::RenderTarget* target) override {
        target->draw(shape);
    }
};

int main(int argc, char *argv[]) {
    simpleengine::Game game(500, 500, "First Example");
    game.AddEvent(new simpleengine::EntityEvent(game.GetWindow(), std::make_unique<SnakePlayerEntity>(game.GetWindow()->getSize())));

    return game.Run();
}