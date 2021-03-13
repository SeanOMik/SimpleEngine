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

#include <SFML/Window/Event.hpp>

#include <functional>

#include "entity.h"

namespace simpleengine {
    class Event;

    class Game {
    public:
        Game(int w, int h, const std::string& window_name);
        Game(const sf::Vector2u& window_size, const std::string& window_name);
        virtual ~Game();

        void UpdateSFMLEvents();
        void Update();
        void RenderWindow();
        void RenderItems();
        void ExitGame();
        int Run();

        void AddEvent(Event* event);
        sf::RenderWindow* GetWindow();

        template<sf::Event::EventType type, class _Fty>
        void AddEventCallback(const std::function<_Fty>& func) {
            if constexpr (type == sf::Event::Closed) {
                close_event.Add(func);
            } else if constexpr (type == sf::Event::Resized) {
                resized_event.Add(func);
            } else if constexpr (type == sf::Event::LostFocus) {
                lost_focus_event.Add(func);
            } else if constexpr (type == sf::Event::GainedFocus) {
                gained_focus_event.Add(func);
            } else if constexpr (type == sf::Event::TextEntered) {
                text_entered_event.Add(func);
            } else if constexpr (type == sf::Event::KeyPressed) {
                key_pressed_event.Add(func);
            } else if constexpr (type == sf::Event::KeyReleased) {
                key_released_event.Add(func);
            } else if constexpr (type == sf::Event::MouseWheelMoved) {
                mouse_wheel_moved_event.Add(func);
            } else if constexpr (type == sf::Event::MouseWheelScrolled) {
                mouse_wheel_scrolled_event.Add(func);
            } else if constexpr (type == sf::Event::MouseButtonPressed) {
                mouse_button_pressed_event.Add(func);
            } else if constexpr (type == sf::Event::MouseButtonReleased) {
                mouse_button_released_event.Add(func);
            } else if constexpr (type == sf::Event::MouseMoved) {
                mouse_move_event.Add(func);
            } else if constexpr (type == sf::Event::MouseEntered) {
                mouse_entered_event.Add(func);
            } else if constexpr (type == sf::Event::MouseLeft) {
                mouse_left_event.Add(func);
            } else if constexpr (type == sf::Event::JoystickButtonPressed) {
                joy_btn_pressed_event.Add(func);
            } else if constexpr (type == sf::Event::JoystickButtonReleased) {
                joy_btn_released_event.Add(func);
            } else if constexpr (type == sf::Event::JoystickMoved) {
                joy_moved_event.Add(func);
            } else if constexpr (type == sf::Event::JoystickConnected) {
                joy_connected_event.Add(func);
            } else if constexpr (type == sf::Event::JoystickDisconnected) {
                joy_disconnected_event.Add(func);
            } else if constexpr (type == sf::Event::TouchBegan) {
                touch_began_event.Add(func);
            } else if constexpr (type == sf::Event::TouchMoved) {
                touch_moved_event.Add(func);
            } else if constexpr (type == sf::Event::TouchEnded) {
                touch_ended_event.Add(func);
            } else if constexpr (type == sf::Event::SensorChanged) {
                sensor_event.Add(func);
            } else if constexpr (type == sf::Event::Count) {

            }
        }
    private:
        template<class _Fty>
        class EventHolder {
        public:
            template<typename... Args>
            void Trigger(Args... args) {
                for(const std::function<_Fty>& func : callbacks) {
                    func(std::forward<Args>(args)...);
                }
            }

            void Add(const std::function<_Fty>& func) {
                callbacks.emplace_back(func);
            }
        private:
            std::vector<std::function<_Fty>> callbacks;
        };

        // Window event callbacks:
        EventHolder<void()> close_event;
        EventHolder<void(sf::Event::SizeEvent)> resized_event;
        EventHolder<void()> lost_focus_event;
        EventHolder<void()> gained_focus_event;
        EventHolder<void(sf::Event::TextEvent)> text_entered_event;
        EventHolder<void(sf::Event::KeyEvent)> key_pressed_event;
        EventHolder<void(sf::Event::KeyEvent)> key_released_event;
        EventHolder<void(sf::Event::MouseWheelEvent)> mouse_wheel_moved_event;
        EventHolder<void(sf::Event::MouseWheelScrollEvent)> mouse_wheel_scrolled_event;
        EventHolder<void(sf::Event::MouseButtonEvent)> mouse_button_pressed_event;
        EventHolder<void(sf::Event::MouseButtonEvent)> mouse_button_released_event;
        EventHolder<void(sf::Event::MouseMoveEvent)> mouse_move_event;
        EventHolder<void()> mouse_entered_event;
        EventHolder<void()> mouse_left_event;
        EventHolder<void(sf::Event::JoystickButtonEvent)> joy_btn_pressed_event;
        EventHolder<void(sf::Event::JoystickButtonEvent)> joy_btn_released_event;
        EventHolder<void(sf::Event::JoystickMoveEvent)> joy_moved_event;
        EventHolder<void(sf::Event::JoystickConnectEvent)> joy_connected_event;
        EventHolder<void(sf::Event::JoystickConnectEvent)> joy_disconnected_event;
        EventHolder<void(sf::Event::TouchEvent)> touch_began_event;
        EventHolder<void(sf::Event::TouchEvent)> touch_moved_event;
        EventHolder<void(sf::Event::TouchEvent)> touch_ended_event;
        EventHolder<void(sf::Event::SensorEvent)> sensor_event;

        sf::RenderWindow* window;

        sf::Clock delta_time_clock; // Delta time clock
        float delta_time; // Delta time

        std::vector<Event*> events;
    };
}

#endif //GAMEENGINE_GAME_H
