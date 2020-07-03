//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "game.h"
#include "event.h"

#include <iostream>

simpleengine::Game::Game(int w, int h, const std::string& window_name) {
    // Create a render window
    window = new sf::RenderWindow(sf::VideoMode(w, h), window_name);
}

simpleengine::Game::~Game() {
    delete window;

    while(!events.empty()) {
        delete events.top();
        events.pop();
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

    if (!events.empty()) {
        events.top()->Update(delta_time);

        // If this state wants to stop, delete it.
        if (events.top()->WantsToQuit()) {
            events.top()->Quiting();

            delete events.top();
            events.pop();
        }
    }
}

void simpleengine::Game::RenderWindow() {
    window->clear();
    RenderItems();
    window->display();
}

void simpleengine::Game::RenderItems() {
    if (!events.empty()) {
        events.top()->Render(window);
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
    events.push(event);
}

sf::RenderWindow* simpleengine::Game::GetWindow() {
    return window;
}

void simpleengine::Game::ExitGame() {
    window->close();
}
