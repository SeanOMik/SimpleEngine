//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include <simpleengine/game.h>
#include <simpleengine/event.h>
#include <simpleengine/entity.h>
#include <simpleengine/components/movement/movement_component.h>
#include <simpleengine/components/animation_component.h>
#include <simpleengine/events/entity_event.h>
#include <simpleengine/animation.h>

#include <chrono>
#include <iostream>

class PlayerEntity : public simpleengine::Entity {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float movement_speed = 95;
    sf::Vector2u window_size;

    std::unique_ptr<simpleengine::AnimationComponent> anim_component;
public:
    explicit PlayerEntity(sf::Vector2u window_size) : window_size(window_size) {
        AddComponent(std::make_unique<simpleengine::MovementComponent>(*this, movement_speed));

        texture.loadFromFile("player_sheet.png");
        sprite.setTexture(texture);

        anim_component = std::make_unique<simpleengine::AnimationComponent>(*this, sprite, texture);
        anim_component->AddAnimation("IDLE_LEFT", 20, 0, 0, 6, 0, 128, 128);
        anim_component->AddAnimation("WALK_LEFT_NO_SWORD", 9, 0, 8, 9, 8, 128, 128);
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

        //anim_component->PlayAnimation("IDLE_LEFT", delta_time);
        anim_component->PlayAnimation("WALK_LEFT_NO_SWORD", delta_time);
    }

    void Render(sf::RenderTarget* target) override {
        target->draw(sprite);
    }
};

int main(int argc, char *argv[]) {
    simpleengine::Game game(500, 500, "SimpleEngine - Animation Example");
    game.AddEvent(new simpleengine::EntityEvent(game.GetWindow(), std::make_unique<PlayerEntity>(game.GetWindow()->getSize())));

    return game.Run();
}