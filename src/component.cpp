//
// Created by SeanOMik on 7/3/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#include "component.h"

simpleengine::Component::Component(Entity& owning_entity) : owning_entity(owning_entity) {

}

void simpleengine::Component::DestroyLater() {
    destroying = true;
}

const bool &simpleengine::Component::IsGettingDestroyed() const {
    return destroying;
}
