//
// Created by SeanOMik on 3/12/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_ASYNC_EVENT_H
#define SIMPLEENGINE_ASYNC_EVENT_H

#include "event.h"

#include <functional>
#include <future>
#include <thread>
#include <iostream>

namespace simpleengine {
    class AsyncEvent : public simpleengine::Event {
    public:
        explicit AsyncEvent(sf::RenderWindow* window = nullptr) : simpleengine::Event(window) {
            update_thread = std::thread( [this]() {
                while (!quit) {
                    std::unique_lock<std::mutex> unique_lock(mutex);
                    cond_var.wait(unique_lock);

                    // After waiting check if we're quiting and if we need to stop this thread.
                    if (quit) break;
            
                    AsyncUpdate(tick_delta_time);
                }
            });
        }

        virtual ~AsyncEvent() {
            quit = true;

            // Notify the async updating thread that we're quiting.
            {
                std::unique_lock<std::mutex> unique_lock(mutex);
                cond_var.notify_all();
            }

            // Quit the async updating thread if its still running.
            if (update_thread.joinable()) {
                update_thread.join();
            }
        };
        
        virtual void AsyncUpdate(const float delta_time) = 0;

        // DO NOT OVERRIDE THIS FUNCTION
        // Notify the update thread to trigger the async update.
        virtual void Update(const float& delta_time) final {
            std::unique_lock<std::mutex> unique_lock(mutex);
            tick_delta_time = delta_time;
            cond_var.notify_all();
        }

        virtual void Render(sf::RenderTarget* target) = 0;
    protected:
        std::mutex mutex;
        std::condition_variable cond_var;
        std::thread update_thread;
        float tick_delta_time;
    };
}

#endif //SIMPLEENGINE_ASYNC_EVENT_H
