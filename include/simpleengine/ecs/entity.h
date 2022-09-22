#pragma once

#include "component/component.h"
#include "../transformable.h"
#include "../util.h"

#include <iterator>
#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <bitset>
#include <type_traits>

namespace simpleengine {
    /**
     * @brief A Model is a object that will be shown on the screen by a renderer.
     * 
     */
    class Entity : public simpleengine::Event, public simpleengine::Transformable {
        // TODO: Don't extend from Event, create own destroy function
    private:
        static uint32_t incrementing_handle;
        uint32_t handle;
    public:
        std::vector<std::shared_ptr<Component>> components;

        Entity() : components({}) {
            handle = incrementing_handle++;
        }

        /* Entity() : components({}) {
            std::random_device rd; // obtain a random number from hardware
            std::mt19937 gen(rd()); // seed the generator
            std::uniform_int_distribution<uint16_t> distr(1, std::numeric_limits<std::uint16_t>::max());

            uint16_t num = distr(gen);
            uint32_t pid = simpleengine::util::get_current_pid();

            handle |= num;
            handle |= (pid << 16);

            std::string binary = std::bitset<16>(num).to_string();
            std::cout << "Entity handle: " << binary << std::endl;
            
        } */

        Entity(std::vector<std::shared_ptr<Component>> components) : Entity() {
            this->components = components;
        }

        uint32_t get_handle() {
            return handle;
        }

        virtual void update(const float& delta_time) override {
            for (auto& component : components) {
                component->update(delta_time);
            }

            rotate_y(delta_time * 10); // TODO: Remove
        }

        template<typename T>
        bool has_component() const {
            for (const auto& comp : components) {
                if (std::dynamic_pointer_cast<T>(comp)) {
                    return true;
                }
            }
            
            return false;
        }

        template<typename T>
        std::shared_ptr<T> get_component() const {
            for (const auto& comp : components) {
                if (std::shared_ptr<T> dyn_comp = std::dynamic_pointer_cast<T>(comp); dyn_comp) {
                    return dyn_comp;
                }
            }

            return nullptr;
        }

        template<typename T>
        void add_component(std::shared_ptr<T> component) {
            static_assert(std::is_base_of_v<Component, T>, "Component class must derive from simpleengine::Component");

            // Only allow one type of the same component
            assert(!has_component<T>()); // TODO: Don't assert, give an error
            components.push_back(component);
        }

        template<typename T>
        void add_component(T component) {
            static_assert(std::is_base_of_v<Component, T>, "Component class must derive from simpleengine::Component");

            // Only allow one type of the same component
            assert(!has_component<T>()); // TODO: Don't assert, give an error
            components.push_back(std::make_shared<T>(component));
        }

        template<typename T, typename ...Args>
        std::shared_ptr<T> add_component(Args&&... args) {
            static_assert(std::is_base_of_v<Component, T>, "Component class must derive from simpleengine::Component");

            // Only allow one type of the same component
            assert(!has_component<T>()); // TODO: Don't assert, give an error
            auto comp = std::make_shared<T>(std::forward<Args>(args)...);
            components.push_back(comp);

            return comp;
        }
    };
}