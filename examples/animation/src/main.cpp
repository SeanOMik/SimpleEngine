//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include <simpleengine/game.h>
#include <simpleengine/event.h>
#include <simpleengine/entity.h>
#include <simpleengine/components/movement/movement_component.h>
#include <simpleengine/components/ssma_component.h>
#include <simpleengine/events/entity_event.h>
#include <simpleengine/animation.h>

#include <chrono>
#include <iostream>

class PlayerEntity : public simpleengine::Entity {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float movement_speed = 150;
    sf::Vector2u window_size;

    std::unique_ptr<simpleengine::SideScrollerMovementAnimationComponent> move_anim_component;
public:
    explicit PlayerEntity(sf::Vector2u window_size) : window_size(window_size) {
        texture.loadFromFile("player_sheet.png");
        sprite.setTexture(texture);

        move_anim_component = std::make_unique<simpleengine::SideScrollerMovementAnimationComponent>(*this, sprite,
                texture, movement_speed, 5, 1.1);
        move_anim_component->SetAnimation(simpleengine::MovementAnimationType::WALK_LEFT, 9, 0, 8,
                9, 8, 128, 128);
        move_anim_component->SetAnimation(simpleengine::MovementAnimationType::IDLE_LEFT, 20, 0, 0,
                6, 0, 128, 128);
        AddComponent(std::move(move_anim_component));
    }

    ~PlayerEntity() override {
        std::cout << "Destroying" << std::endl;

        simpleengine::Entity::~Entity();
    }

    void Move(const float& delta_time, const float& x, const float& y) override {
        sprite.move(x, y);
    }

    void Move(const float& delta_time, const sf::Vector2f& offset) override {
        sprite.move(offset * delta_time);
    }

    void Move(const sf::Vector2f& offset) override {
        sprite.move(offset);
    }

    void Update(const float& delta_time) override {
        Entity::Update(delta_time);
    }

    void Render(sf::RenderTarget* target) override {
        target->draw(sprite);
    }
};

int main(int argc, char *argv[]) {
    simpleengine::Game game(700, 700, "SimpleEngine - Animation Example");
    game.AddEvent(new simpleengine::EntityEvent(game.GetWindow(), std::make_unique<PlayerEntity>(game.GetWindow()->getSize())));

    return game.Run();
}