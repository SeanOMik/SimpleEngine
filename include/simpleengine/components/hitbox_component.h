//
// Created by SeanOMik on 7/6/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_HITBOX_COMPONENT_H
#define SIMPLEENGINE_HITBOX_COMPONENT_H

#include "../component.h"

#include <SFML/Graphics.hpp>

namespace simpleengine {
    class HitboxComponent : public Component {
    private:
        sf::Sprite& sprite;
        sf::RectangleShape hitbox;

        float offset_x;
        float offset_y;
    public:
        HitboxComponent(Entity& owning_entity, sf::Sprite& sprite, float sprite_offset_x, float sprite_offset_y,
                float width, float height);

        bool DoesIntersect(const sf::FloatRect& rect);

        void Update(const float &delta_time) override;
        void Render(sf::RenderTarget *target) override;
    };
}

#endif //SIMPLEENGINE_HITBOX_COMPONENT_H
