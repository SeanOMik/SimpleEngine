//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_GAME_H
#define SIMPLEENGINE_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <stack>

namespace simpleengine {
    class Event;

    class Game {
    public:
        Game(int w, int h, const std::string& window_name);
        virtual ~Game();

        void UpdateSFMLEvents();
        void Update();
        void RenderWindow();
        void RenderItems();
        void ExitGame();
        int Run();

        void AddEvent(Event* event);
        sf::RenderWindow* GetWindow();
    private:
        sf::RenderWindow* window;

        sf::Clock delta_time_clock; // Delta time clock
        float delta_time; // Delta time

        std::stack<Event*> events;
    };
}

#endif //GAMEENGINE_GAME_H
