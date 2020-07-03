//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_MOVEMENT_COMPONENT_H
#define SIMPLEENGINE_MOVEMENT_COMPONENT_H

#include "../../component.h"

#include <SFML/Graphics.hpp>

namespace simpleengine {
    class Entity;

    class MovementComponent : public Component {
    public:
        explicit MovementComponent(Entity* owning_entity, float movement_speed);

        void Update(const float &delta_time) override;
    private:
        float movement_speed;
    };
}

#endif //SIMPLEENGINE_MOVEMENT_COMPONENT_H
