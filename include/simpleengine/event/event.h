#pragma once

#include "../destructable.h"

#include <memory>

namespace simpleengine {
    class Event : public simpleengine::Destructable {
    public:
        Event() = default;
        virtual ~Event() = default;
        
        virtual void update(const float& delta_time) = 0;
    };
}