//
// Created by SeanOMik on 7/5/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_ANIMATION_H
#define SIMPLEENGINE_ANIMATION_H

#include <SFML/Graphics.hpp>

namespace simpleengine {
    class Animation {
    public:
        Animation(sf::Sprite &sprite, sf::Texture &texture_sheet, float speed, int start_frame_x,
                int start_frame_y, int frame_ct_x, int frame_ct_y, int width, int height);

        void Update(const float& delta_time);
        void Reset();

        void FlipHorizontally();
        void FlipVertically();

        bool IsHorizontallyFlipped() const;
        bool IsVerticallyFlipped() const;

        /*void Play();
        void Pause();*/
    private:
        sf::Sprite &sprite;
        sf::Texture &texture_sheet;
        float speed; // Speed is how long it takes until it swaps to next texture.
        float timer;
        int width;
        int height;
        sf::IntRect start_rect;
        sf::IntRect current_rect;
        sf::IntRect end_rect;

        bool horizontally_flipped = false;
        bool vertically_flipped = false;
    };
}

#endif //SIMPLEENGINE_ANIMATION_H
