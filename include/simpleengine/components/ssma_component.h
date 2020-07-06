//
// Created by SeanOMik on 7/5/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_SSMA_COMPONENT_H
#define SIMPLEENGINE_SSMA_COMPONENT_H

#include "../component.h"
#include "animation_component.h"
#include "movement/movement_component.h"

#include <SFML/Graphics.hpp>

namespace simpleengine {
    class Entity;
    class Animation;

    enum MovementAnimationType {
        WALK_LEFT,
        WALK_RIGHT,
        IDLE_LEFT,
        IDLE_RIGHT
    };

    // Use this for side scrollers!
    class SideScrollerMovementAnimationComponent : public Component {
    public:
        SideScrollerMovementAnimationComponent(Entity& owning_entity, sf::Sprite &sprite, sf::Texture &texture_sheet,
                                               float max_velocity, float acceleration, float deceleration);

        void SetAnimation(const MovementAnimationType& type, simpleengine::Animation animation);
        void SetAnimation(const MovementAnimationType& type, float speed, int start_frame_x, int start_frame_y, int frame_ct_x, int frame_ct_y,
                int width, int height);

        void Update(const float& delta_time) override;

        simpleengine::AnimationComponent GetAnimationComponent();
        simpleengine::MovementComponent GetMovementComponent();
    private:
        std::string MovementAnimationTypeToStr(const MovementAnimationType& type);

        simpleengine::AnimationComponent anim_component;
        simpleengine::MovementComponent move_component;
    };
}


#endif //SIMPLEENGINE_SSMA_COMPONENT_H
