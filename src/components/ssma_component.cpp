//
// Created by SeanOMik on 7/5/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "components/ssma_component.h"

#include <iostream>

simpleengine::SideScrollerMovementAnimationComponent::SideScrollerMovementAnimationComponent(simpleengine::Entity &owning_entity,
        sf::Sprite &sprite, sf::Texture &texture_sheet, float max_velocity, float acceleration, float deceleration)
        : Component(owning_entity), anim_component(owning_entity, sprite, texture_sheet),
        move_component(owning_entity, max_velocity, acceleration, deceleration) {
}

void simpleengine::SideScrollerMovementAnimationComponent::SetAnimation(const simpleengine::MovementAnimationType &type,
        simpleengine::Animation animation) {

    anim_component.AddAnimation(MovementAnimationTypeToStr(type), animation);
}

void simpleengine::SideScrollerMovementAnimationComponent::SetAnimation(const simpleengine::MovementAnimationType &type,
        float speed, int start_frame_x, int start_frame_y, int frame_ct_x, int frame_ct_y, int width, int height) {

    anim_component.AddAnimation(MovementAnimationTypeToStr(type), speed, start_frame_x, start_frame_y,
            frame_ct_x, frame_ct_y, width, height);
}


void simpleengine::SideScrollerMovementAnimationComponent::Update(const float& delta_time) {
    move_component.Update(delta_time);

    if (move_component.GetVelocity().x > 0) { // Moving right
        // If the user never set a WALK_RIGHT animation, then we need to flip the WALK_LEFT one.
        if (anim_component.HasAnimation("WALK_RIGHT")) {
            Animation& anim = anim_component.GetAnimation("WALK_RIGHT");
            if (anim.IsHorizontallyFlipped()) {
                anim.FlipHorizontally();
            }

            anim.Update(delta_time);
        } else {
            Animation& anim = anim_component.GetAnimation("WALK_LEFT");
            if (!anim.IsHorizontallyFlipped()) {
                anim.FlipHorizontally();
            }
            anim.Update(delta_time);
        }
    } else if (move_component.GetVelocity().x < 0) { // Moving left
        // If the user never set a WALK_LEFT animation, then we need to flip the WALK_RIGHT one.
        if (anim_component.HasAnimation("WALK_LEFT")) {
            Animation& anim = anim_component.GetAnimation("WALK_LEFT");
            if (anim.IsHorizontallyFlipped()) {
                anim.FlipHorizontally();
            }

            anim.Update(delta_time);
        } else {
            Animation& anim = anim_component.GetAnimation("WALK_RIGHT");
            if (!anim.IsHorizontallyFlipped()) {
                anim.FlipHorizontally();
            }
            anim.Update(delta_time);
        }
    } else {
        // If the user never set a IDLE_RIGHT animation, then we need to flip the IDLE_LEFT one.
        if (move_component.GetLastDirection().x > 0) { // Facing right
            if (anim_component.HasAnimation("IDLE_RIGHT")) {
                Animation& anim = anim_component.GetAnimation("IDLE_RIGHT");
                if (anim.IsHorizontallyFlipped()) {
                    anim.FlipHorizontally();
                }

                anim.Update(delta_time);
            } else {
                Animation& anim = anim_component.GetAnimation("IDLE_LEFT");
                anim.Update(delta_time);
            }
        } else {
            // If the user never set a IDLE_LEFT animation, then we need to flip the IDLE_RIGHT one.
            if (anim_component.HasAnimation("IDLE_LEFT")) { // Facing left
                Animation& anim = anim_component.GetAnimation("IDLE_LEFT");
                if (anim.IsHorizontallyFlipped()) {
                    anim.FlipHorizontally();
                }

                anim.Update(delta_time);
            } else {
                Animation& anim = anim_component.GetAnimation("IDLE_RIGHT");
                anim.Update(delta_time);
            }
        }
    }
}

std::string simpleengine::SideScrollerMovementAnimationComponent::MovementAnimationTypeToStr(
        const simpleengine::MovementAnimationType &type) {

    switch (type) {
        case WALK_LEFT:
            return "WALK_LEFT";
        case WALK_RIGHT:
            return "WALK_RIGHT";
        case IDLE_LEFT:
            return "IDLE_LEFT";
        case IDLE_RIGHT:
            return "IDLE_RIGHT";
        default:
            return "UNKNOWN_MOVEMENT_ANIMATION_TYPE";
    }
}

simpleengine::AnimationComponent& simpleengine::SideScrollerMovementAnimationComponent::GetAnimationComponent() {
    return anim_component;
}

simpleengine::MovementComponent& simpleengine::SideScrollerMovementAnimationComponent::GetMovementComponent() {
    return move_component;
}
