//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_EVENT_H
#define SIMPLEENGINE_EVENT_H

#include <SFML/Graphics.hpp>

namespace simpleengine {
    class Event {
    public:
        explicit Event(sf::RenderWindow* window = nullptr) : window(window) {}
        virtual ~Event() = default;

        // Check if this event is quitting.
        virtual const bool& IsQuitting() {
            return quit;
        }

        // Quit the event at next tick.
        virtual void Quit() {
            quit = true;
        }

        // Abstract methods
        //virtual void CheckForQuit() = 0; // Ran every Update to check if we're gonna quit.
        //virtual void Quiting() {} // Ran when a State is about to be destroyed.
        virtual void Update(const float& delta_time) = 0;
        virtual void Render(sf::RenderTarget* target) = 0;
    protected:
        sf::RenderWindow* window;
        bool quit = false;
    };
}

#endif //GAMEENGINE_EVENT_H
