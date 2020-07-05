//
// Created by SeanOMik on 7/5/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "animation.h"

simpleengine::Animation::Animation(sf::Sprite &sprite, sf::Texture &texture_sheet, float speed, int start_frame_x,
    int start_frame_y, int frame_ct_x, int frame_ct_y, int width, int height) : sprite(sprite), texture_sheet(texture_sheet),
        speed(speed), start_rect(sf::IntRect(start_frame_x * width, start_frame_y * height, width, height)),
        end_rect(sf::IntRect(frame_ct_x * width, frame_ct_y * height, width, height)), width(width), height(height) {

    timer = 0;

    // Set start of sprite.
    this->sprite.setTexture(texture_sheet, true);
    this->sprite.setTextureRect(start_rect);
    this->current_rect = start_rect;
}

void simpleengine::Animation::Update(const float &delta_time) {
    // Update timer
    timer += 100 * delta_time;
    if (timer >= speed) {
        // Reset animation
        timer = 0;

        // We have not reached the end, so change the texture.
        if (current_rect != end_rect) {
            current_rect.left += width;
        } else {
            current_rect.left = start_rect.left;
        }

        sprite.setTextureRect(current_rect);
    }
}

void simpleengine::Animation::Reset() {
    timer = 0;
    current_rect = start_rect;
}
