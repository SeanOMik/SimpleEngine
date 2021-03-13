//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <functional>
#include <ios>

#include <simpleengine/game.h>

#include <simpleengine/event.h>
#include <simpleengine/async_event.h>

#include <simpleengine/entity.h>

// Simple engine components
#include <simpleengine/components/collision_component.h>
#include <simpleengine/components/movement/movement_component.h>

// Simpleengine events
#include <simpleengine/events/entity_event.h>
#include <simpleengine/events/collision_handler.h>

// Simple engine utils
#include <simpleengine/random.h>

#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>

class SnakeMovementComponent : public simpleengine::Component {
private:
    float movement_speed = 15;
    sf::Vector2i movement_direction;
    std::chrono::high_resolution_clock::time_point last_movement;
    sf::Vector2u window_size;
    const sf::Vector2f& entity_pos;

    simpleengine::Game& game;
public:
    explicit SnakeMovementComponent(simpleengine::Entity& owning_entity, float movement_speed, const sf::Vector2u& window_size, simpleengine::Game& game)
        : simpleengine::Component(owning_entity), movement_speed(movement_speed), window_size(window_size),
            entity_pos(owning_entity.GetTransformable().getPosition()), game(game) {

        game.AddEventCallback<sf::Event::EventType::KeyPressed>(std::function([this](sf::Event::KeyEvent event) {
            this->OnKeyPress(event);
        }));
    }

    void OnKeyPress(sf::Event::KeyEvent event) {
        sf::Keyboard::Key key = event.code;

        switch (key) {
            case sf::Keyboard::W:
                movement_direction.x = 0;
                movement_direction.y = -15;
                break;
            case sf::Keyboard::A:
                movement_direction.x = -15;
                movement_direction.y = 0;
                break;
            case sf::Keyboard::S:
                movement_direction.x = 0;
                movement_direction.y = 15;
                break;
            case sf::Keyboard::D:
                movement_direction.x = 15;
                movement_direction.y = 0;
                break;
            case sf::Keyboard::Space:
                movement_direction.x = 0;
                movement_direction.y = 0;
                break;
            default:
                break;
        }
    }

    const sf::Vector2i& GetDirection() {
        return movement_direction;
    }

    void Update(const float& delta_time) override {
        long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - last_movement).count();

        // Only move every 115 ticks.
        if (duration >= 115 && (movement_direction.x != 0 || movement_direction.y != 0)) {
            owning_entity.Move(delta_time, movement_direction.x, movement_direction.y);
            last_movement = std::chrono::high_resolution_clock::now();

            sf::RectangleShape& shape = dynamic_cast<sf::RectangleShape&>(owning_entity.GetTransformable());

            // Wrap entity around the side of the screen.
            if (entity_pos.x < 0) {
                sf::Vector2f new_pos(window_size.x - shape.getSize().x, owning_entity.GetTransformable().getPosition().y);
                owning_entity.GetTransformable().setPosition(new_pos);
            } else if (entity_pos.x > window_size.x) {
                sf::Vector2f new_pos(shape.getSize().x, owning_entity.GetTransformable().getPosition().y);
                owning_entity.GetTransformable().setPosition(new_pos);
            }

            if (entity_pos.y < 0) {
                sf::Vector2f new_pos(owning_entity.GetTransformable().getPosition().x, window_size.y - shape.getSize().y);
                owning_entity.GetTransformable().setPosition(new_pos);
            } else if (entity_pos.y > window_size.y) {
                sf::Vector2f new_pos(owning_entity.GetTransformable().getPosition().x, shape.getSize().y);
                owning_entity.GetTransformable().setPosition(new_pos);
            }
        }
    }
};

class SnakePlayerEntity;

class SnakeFoodEntity : public simpleengine::Entity {
private:
    sf::RectangleShape shape;
    sf::Vector2u window_size;
    std::shared_ptr<SnakePlayerEntity> snake_player;

    std::shared_ptr<simpleengine::CollisionComponent> collision_component;
public:
    explicit SnakeFoodEntity(sf::Vector2u window_size, std::shared_ptr<SnakePlayerEntity> snake_player)
            : Entity(shape), window_size(window_size), snake_player(snake_player) {
        
        shape.setSize(sf::Vector2f(15, 15));
        shape.setPosition(45, 45);
        shape.setFillColor(sf::Color::Red);

        collision_component = std::make_shared<simpleengine::CollisionComponent>(*this, shape, 0, 0, 15, 15);
        AddComponent(collision_component);
    }

    void Move(const float& delta_time, const float& x, const float& y) override {
        shape.move(x, y);
    }

    void Move(const float& delta_time, const sf::Vector2f& offset) override {
        shape.move(offset * delta_time);
    }

    void Move(const sf::Vector2f& offset) override {
        shape.move(offset);
    }

    void Render(sf::RenderTarget* target) override {
        target->draw(shape);
    }

    // Find a new location for the snake food.
    void Relocate() {
        simpleengine::Random<int> random;
        sf::Vector2f pos(random.NextInRange(0, window_size.x), random.NextInRange(0, window_size.y));

        // Make sure its on the 15 pixel grid.
        pos.x -= (int) pos.x % 15;
        pos.y -= (int) pos.y % 15;

        shape.setPosition(pos);
    }
};

struct BodyPiece {
    sf::RectangleShape shape;
    sf::Vector2i movement_direction;
};

class SnakePlayerEntity : public simpleengine::Entity {
private:
    sf::RectangleShape head;
    
    std::vector<BodyPiece> body;

    float movement_speed = 15;
    sf::Vector2u window_size;
    sf::Text& score_text;

    std::shared_ptr<SnakeMovementComponent> movement_component;
    std::shared_ptr<simpleengine::CollisionComponent> collision_component;

    bool alive = true;
public:
    explicit SnakePlayerEntity(sf::Vector2u window_size, sf::Text& score_text, simpleengine::Game& game) :
            Entity(head), window_size(window_size), score_text(score_text) {
        head = sf::RectangleShape(sf::Vector2f(15, 15));
        head.setFillColor(sf::Color(220, 220, 220));
    
        movement_component = std::make_shared<SnakeMovementComponent>(*this, movement_speed, window_size, game);
        AddComponent(movement_component);

        collision_component = std::make_shared<simpleengine::CollisionComponent>(*this, head, 0, 0, 15, 15);
        collision_component->SetOnCollide(std::bind(&SnakePlayerEntity::OnCollide, this, std::placeholders::_1));
        AddComponent(collision_component);
    }

    void Move(const float& delta_time, const float& x, const float& y) override {
        if (alive) {
            sf::FloatRect rect(head.getPosition(), head.getSize());
            // Add movement to position
            rect.left += x;
            rect.top += y;

            for (BodyPiece& piece : body) {
                if (piece.movement_direction != sf::Vector2i(0, 0) && rect.intersects(piece.shape.getGlobalBounds())) {
                    alive = false;
                    break;
                }
            }

            if (!alive) return;

            head.move(x, y);

            sf::Vector2i last_movement_dir(x, y);
            for (BodyPiece& piece : body) {
                piece.shape.move(piece.movement_direction.x, piece.movement_direction.y);

                std::swap(last_movement_dir, piece.movement_direction);

                // Wrap piece around the side of the screen.
                sf::Vector2f piece_pos = piece.shape.getPosition();
                if (piece_pos.x < 0) {
                    sf::Vector2f new_pos(window_size.x - piece.shape.getSize().x, piece_pos.y);
                    piece.shape.setPosition(new_pos);
                } else if (piece_pos.x > window_size.x) {
                    sf::Vector2f new_pos(piece.shape.getSize().x, piece_pos.y);
                    piece.shape.setPosition(new_pos);
                }

                if (piece_pos.y < 0) {
                    sf::Vector2f new_pos(piece_pos.x, window_size.y - piece.shape.getSize().y);
                    piece.shape.setPosition(new_pos);
                } else if (piece_pos.y > window_size.y) {
                    sf::Vector2f new_pos(piece_pos.x, piece.shape.getSize().y);
                    piece.shape.setPosition(new_pos);
                }
            }
        }
    }

    void Move(const float& delta_time, const sf::Vector2f& offset) override {
        head.move(offset * delta_time);
    }

    void Move(const sf::Vector2f& offset) override {
        head.move(offset);
    }

    void Render(sf::RenderTarget* target) override {
        target->draw(score_text);
        target->draw(head);

        for (const BodyPiece& piece : body) {
            target->draw(piece.shape);
        }
    }

    void OnCollide(std::shared_ptr<Entity> entity) {
        if (std::shared_ptr<SnakeFoodEntity> shared = std::dynamic_pointer_cast<SnakeFoodEntity>(entity); shared) {
            shared->Relocate();

            BodyPiece piece{ sf::RectangleShape(sf::Vector2f(15, 15)), sf::Vector2i(0, 0) };
            piece.shape.setFillColor(sf::Color::White);

            if (body.empty()) {
                piece.shape.setPosition(head.getPosition());
            } else {
                piece.shape.setPosition(body.back().shape.getPosition());
            }

            body.push_back(piece);
            score_text.setString(std::to_string(body.size()));
        }
    }
};

int main(int argc, char *argv[]) {
    sf::Vector2u window_size(495, 495);
    simpleengine::Game game(window_size, "SimpleEngine - Snake Example");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Failed to find \"arial.ttf\"" << std::endl;
        return 0;
    }

    sf::Text score_text;
    score_text.setFont(font);
    score_text.setString("0");
    score_text.setCharacterSize(24);
    score_text.setFillColor(sf::Color::Green);
    score_text.setPosition(window_size.x - 50, 5);

    auto snake_player = std::make_shared<SnakePlayerEntity>(window_size, score_text, std::ref(game));
    auto snake_food = std::make_shared<SnakeFoodEntity>(window_size, snake_player);
    game.AddEvent(new simpleengine::EntityEvent(game.GetWindow(), snake_food));
    game.AddEvent(new simpleengine::EntityEvent(game.GetWindow(), snake_player));
    game.AddEvent(new simpleengine::CollisionHandler(game.GetWindow(), { snake_player, snake_food } ));

    return game.Run();
}