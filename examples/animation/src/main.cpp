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
#include <simpleengine/components/collision_component.h>
#include <simpleengine/animation.h>

#include <chrono>
#include <iostream>

class PlayerEntity : public simpleengine::Entity {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float movement_speed = 130;
    sf::Vector2u window_size;

    // Components:
    std::shared_ptr<simpleengine::CollisionComponent> hitbox_component;
    std::shared_ptr<simpleengine::SideScrollerMovementAnimationComponent> move_anim_component;
public:
    explicit PlayerEntity(sf::Vector2u window_size) : Entity(sprite), window_size(window_size) {
        texture.loadFromFile("player_sheet.png");
        texture.setSmooth(true);
        sprite.setTexture(texture);
        sprite.setScale(0.7f, 0.7f);

        move_anim_component = std::make_shared<simpleengine::SideScrollerMovementAnimationComponent>(*this, sprite,
            texture, movement_speed, 5.f, 1.1f);
        move_anim_component->SetAnimation(simpleengine::MovementAnimationType::WALK_LEFT, 8, 0, 9,
            9, 9, 128, 128);
        move_anim_component->SetAnimation(simpleengine::MovementAnimationType::IDLE_LEFT, 20, 0, 0,
            6, 0, 128, 128);
        AddComponent(std::move(move_anim_component));
        AddComponent(move_anim_component);

        hitbox_component = std::make_shared<simpleengine::CollisionComponent>(*this, sprite,
                                                                              20.f, 12.f, sprite.getGlobalBounds().width - 40.f, sprite.getGlobalBounds().height - 15.f);
        AddComponent(hitbox_component);
    }

    ~PlayerEntity() override {
        std::cout << "Destroying" << std::endl;

        simpleengine::Entity::~Entity();
    }

    void Update(const float& delta_time) override {
        Entity::Update(delta_time);
    }

    void Render(sf::RenderTarget* target) override {
        Entity::Render(target);

        target->draw(sprite);
    }
};

int main(int argc, char *argv[]) {
    simpleengine::Game game(700, 700, "SimpleEngine - Animation Example");
    game.AddEvent(new simpleengine::EntityEvent(game.GetWindow(), std::make_unique<PlayerEntity>(game.GetWindow()->getSize())));

    return game.Run();
}