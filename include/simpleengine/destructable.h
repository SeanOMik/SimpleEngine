//
// Created by SeanOMik on 3/12/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_DESTRUCTABLE_H
#define SIMPLEENGINE_DESTRUCTABLE_H

namespace simpleengine {
    class Destructable {
    public:
        virtual void Destroy() {
            destroying = true;
        }

        virtual const bool& IsDestroying() const {
            return destroying;
        }
    protected:
        bool destroying = false;
    };
}

#endif //SIMPLEENGINE_DESTRUCTABLE_H
