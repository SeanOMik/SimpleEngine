//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "game.h"
#include "entity.h"
#include "event.h"

#include <iostream>

simpleengine::Game::Game(int w, int h, const std::string& window_name) {
    // Create a render window
    window = new sf::RenderWindow(sf::VideoMode(w, h), window_name);
}

simpleengine::Game::Game(const sf::Vector2u& window_size, const std::string& window_name) : simpleengine::Game(window_size.x, window_size.y, window_name) {

}

simpleengine::Game::~Game() {
    delete window;

    /* while(!events.empty()) {
        delete events.top();
        events.pop();
    } */

    std::vector<Event*>::iterator it = events.begin();
    while (it != events.end()) {
        delete (*it);
        it = events.erase(it);
    }
}

void simpleengine::Game::UpdateSFMLEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window->close();
        }
    }
}

void simpleengine::Game::Update() {
    delta_time = delta_time_clock.restart().asSeconds(); // Update delta time
    UpdateSFMLEvents();

    for (std::vector<Event*>::iterator it = events.begin(); it != events.end(); ) {
        (*it)->Update(delta_time);

        if ((*it)->IsQuitting()) {
            delete (*it);
            it = events.erase(it);
        } else {
            ++it;
        }
    }
}

void simpleengine::Game::RenderWindow() {
    window->clear();
    RenderItems();
    window->display();
}

void simpleengine::Game::RenderItems() {
    for (std::vector<Event*>::iterator it = events.begin(); it != events.end(); it++) {
        (*it)->Render(window);
    }
}

int simpleengine::Game::Run() {
    sf::CircleShape shape(100);
    shape.setFillColor(sf::Color::Green);

    while (window->isOpen()) {
        Update();
        RenderWindow();
    }

    return 0;
}

void simpleengine::Game::AddEvent(simpleengine::Event *event) {
    events.emplace_back(event);
}

sf::RenderWindow* simpleengine::Game::GetWindow() {
    return window;
}

void simpleengine::Game::ExitGame() {
    window->close();
}
