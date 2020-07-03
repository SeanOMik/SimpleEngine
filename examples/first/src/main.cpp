//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include <iostream>

#include <gameengine/game.h>
#include <gameengine/event.h>
#include <gameengine/entity.h>

class PlayerEntity : public simpleengine::Entity {
private:
    sf::RectangleShape shape;
    float movement_speed;
public:
    PlayerEntity() {
        shape = sf::RectangleShape(sf::Vector2f(50, 50));
        shape.setFillColor(sf::Color::White);

        movement_speed = 250;
    }

    void Move(const float& delta_time, const float& x, const float& y) override {
        shape.move(x * movement_speed * delta_time, y * movement_speed * delta_time);
    }

    void Update(const float& delta_time) override {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            Move(delta_time, -1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            Move(delta_time, 0, -1);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            Move(delta_time, 1, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            Move(delta_time, 0, 1);
        }
    }

    void Render(sf::RenderTarget* target = nullptr) override {
        target->draw(shape);
    }
};

class PlayerEvent : public simpleengine::Event {
private:
    PlayerEntity entity;
public:
    explicit PlayerEvent(sf::RenderWindow* window = nullptr) : simpleengine::Event(window) {}

    void CheckForQuit() override {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            quit = true;
        }
    }

    void Update(const float& delta_time) override {
        entity.Update(delta_time);
    }

    void Render(sf::RenderTarget* target = nullptr) override {
        entity.Render(target);
    }
};

int main(int argc, char *argv[]) {
    simpleengine::Game game(500, 500, "First Example");
    game.AddEvent(new PlayerEvent());

    return game.Run();
}