//
// Created by SeanOMik on 7/4/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "components/animation_component.h"
#include "animation.h"

simpleengine::AnimationComponent::AnimationComponent(Entity& owning_entity, sf::Sprite& sprite, sf::Texture& texture_sheet)
    : Component(owning_entity), sprite(sprite), texture_sheet(texture_sheet) {

}

void simpleengine::AnimationComponent::Update(const float& delta_time) {

}

void simpleengine::AnimationComponent::AddAnimation(const std::string& animation_name, float speed, int start_frame_x, int start_frame_y, int frame_ct_x,
    int frame_ct_y, int width, int height) {

    animations.emplace(animation_name, Animation(sprite, texture_sheet, speed, start_frame_x, start_frame_y, frame_ct_x, frame_ct_y, width, height));
}

void simpleengine::AnimationComponent::AddAnimation(const std::string& animation_name, simpleengine::Animation animation) {
    animations.emplace(animation_name, animation);
}

void simpleengine::AnimationComponent::UpdateAnimation(const std::string &animation_name, const float& delta_time) {
    auto anim = animations.find(animation_name);
    if (anim != animations.end()) {
        anim->second.Update(delta_time);
    } else {
        throw std::runtime_error("Animation, \"" + animation_name + "\", was not found!");
    }
}

sf::Sprite &simpleengine::AnimationComponent::GetSprite() const {
    return sprite;
}

sf::Texture &simpleengine::AnimationComponent::GetTextureSheet() const {
    return texture_sheet;
}

const std::map<std::string, simpleengine::Animation> &simpleengine::AnimationComponent::GetAnimations() const {
    return animations;
}

/*void simpleengine::AnimationComponent::FlipAnimationHorizontally(const std::string &animation_name) {
    auto anim = animations.find(animation_name);
    if (anim != animations.end()) {
        anim->second.FlipHorizontally();
    } else {
        throw std::runtime_error("Animation, \"" + animation_name + "\", was not found!");
    }
}

void simpleengine::AnimationComponent::FlipAnimationVertically(const std::string &animation_name) {
    auto anim = animations.find(animation_name);
    if (anim != animations.end()) {
        anim->second.FlipVertically();
    } else {
        throw std::runtime_error("Animation, \"" + animation_name + "\", was not found!");
    }
}

void simpleengine::AnimationComponent::IsAnimationFlippedHorizontally(const std::string &animation_name) {
    auto anim = animations.find(animation_name);
    if (anim != animations.end()) {
        return anim->second.IsHorizontallyFlipped();
    } else {
        throw std::runtime_error("Animation, \"" + animation_name + "\", was not found!");
    }
}

void simpleengine::AnimationComponent::IsAnimationFlippedVertically(const std::string &animation_name) {
    auto anim = animations.find(animation_name);
    if (anim != animations.end()) {
        return anim->second.IsHorizontallyFlipped();
    } else {
        throw std::runtime_error("Animation, \"" + animation_name + "\", was not found!");
    }
}*/

simpleengine::Animation& simpleengine::AnimationComponent::GetAnimation(const std::string &animation_name) {
    auto anim = animations.find(animation_name);
    if (anim != animations.end()) {
        return anim->second;
    } else {
        throw std::runtime_error("Animation, \"" + animation_name + "\", was not found!");
    }
}


bool simpleengine::AnimationComponent::HasAnimation(const std::string &animation_name) {
    auto anim = animations.find(animation_name);
    return anim != animations.end();
}