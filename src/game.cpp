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
                close_event.Trigger();
                window->close();
                break;
            case sf::Event::Resized:
                resized_event.Trigger(event.size);
                break;
            case sf::Event::LostFocus:
                lost_focus_event.Trigger();
                break;
            case sf::Event::GainedFocus:
                gained_focus_event.Trigger();
                break;
            case sf::Event::TextEntered:
                text_entered_event.Trigger(event.text);
                break;
            case sf::Event::KeyPressed:
                key_pressed_event.Trigger(event.key);
                break;
            case sf::Event::KeyReleased:
                key_released_event.Trigger(event.key);
                break;
            case sf::Event::MouseWheelMoved:
                mouse_wheel_moved_event.Trigger(event.mouseWheel);
                break;
            case sf::Event::MouseWheelScrolled:
                mouse_wheel_scrolled_event.Trigger(event.mouseWheelScroll);
                break;
            case sf::Event::MouseButtonPressed:
                mouse_button_pressed_event.Trigger(event.mouseButton);
                break;
            case sf::Event::MouseButtonReleased:
                mouse_button_released_event.Trigger(event.mouseButton);
                break;
            case sf::Event::MouseMoved:
                mouse_move_event.Trigger(event.mouseMove);
                break;
            case sf::Event::MouseEntered:
                mouse_entered_event.Trigger();
                break;
            case sf::Event::MouseLeft:
                mouse_left_event.Trigger();
                break;
            case sf::Event::JoystickButtonPressed:
                joy_btn_pressed_event.Trigger(event.joystickButton);
                break;
            case sf::Event::JoystickButtonReleased:
                joy_btn_released_event.Trigger(event.joystickButton);
                break;
            case sf::Event::JoystickMoved:
                joy_moved_event.Trigger(event.joystickMove);
                break;
            case sf::Event::JoystickConnected:
                joy_connected_event.Trigger(event.joystickConnect);
                break;
            case sf::Event::JoystickDisconnected:
                joy_disconnected_event.Trigger(event.joystickConnect);
                break;
            case sf::Event::TouchBegan:
                touch_began_event.Trigger(event.touch);
                break;
            case sf::Event::TouchMoved:
                touch_moved_event.Trigger(event.touch);
                break;
            case sf::Event::TouchEnded:
                touch_ended_event.Trigger(event.touch);
                break;
            case sf::Event::SensorChanged:
                sensor_event.Trigger(event.sensor);
                break;
            case sf::Event::Count:
                break;
        }
    }
}

void simpleengine::Game::Update() {
    delta_time = delta_time_clock.restart().asSeconds(); // Update delta time
    UpdateSFMLEvents();

    for (std::vector<Event*>::iterator it = events.begin(); it != events.end(); ) {
        (*it)->Update(delta_time);

        if ((*it)->IsDestroying()) {
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
