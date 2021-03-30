//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_EVENT_H
#define SIMPLEENGINE_EVENT_H

#include "destructable.h"

#include <SFML/Graphics.hpp>

namespace simpleengine {
    class Event : public simpleengine::Destructable {
    public:
        explicit Event(sf::RenderWindow* window = nullptr) : window(window) {}
        virtual ~Event() = default;
        
        virtual void Update(const float& delta_time) = 0;
        virtual void Render(sf::RenderTarget* target) = 0;
    protected:
        sf::RenderWindow* window;
    };
}

#endif //GAMEENGINE_EVENT_H
