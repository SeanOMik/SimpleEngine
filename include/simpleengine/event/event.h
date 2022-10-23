#pragma once

#include "../destructable.h"

#include <memory>

namespace simpleengine {
    class Event : public simpleengine::Destructable {
    public:
        Event() = default;
        virtual ~Event() = default;
        
        /**
         * @brief The update function with fixed-timestep.
         * 
         * Since this is fixed timestep, this function is primarily for game-logic and physics updates.
         *
         * @param delta_time 
         */
        virtual void update(const float& delta_time) = 0;

        /**
         * @brief The update function with varying-timestep.
         *
         * Since this has a varying timestep, this function is primarily for input related updates.
         * 
         * @param delta_time 
         */
        virtual void input_update(const float& delta_time) {}
    };
}