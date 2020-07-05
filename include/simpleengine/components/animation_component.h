//
// Created by SeanOMik on 7/4/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_ANIMATION_COMPONENT_H
#define SIMPLEENGINE_ANIMATION_COMPONENT_H

#include "../component.h"
#include "../animation.h"

#include <SFML/Graphics.hpp>

namespace simpleengine {
    class Entity;

    class AnimationComponent : public Component {
    private:

        sf::Sprite& sprite;
        sf::Texture& texture_sheet;
        std::map<std::string, Animation> animations;

    public:
        explicit AnimationComponent(Entity& owning_entity, sf::Sprite& sprite, sf::Texture& texture_sheet);

        void Update(const float &delta_time) override;

        void AddAnimation(const std::string& animation_name, float speed, int start_frame_x, int start_frame_y, int frame_ct_x,
            int frame_ct_y, int width, int height);
        void AddAnimation(const std::string& animation_name, simpleengine::Animation animation);
        void UpdateAnimation(const std::string& animation_name, const float& delta_time);

        sf::Sprite &GetSprite() const;
        sf::Texture &GetTextureSheet() const;
        const std::map<std::string, Animation> &GetAnimations() const;
    };
}

#endif //SIMPLEENGINE_ANIMATION_COMPONENT_H