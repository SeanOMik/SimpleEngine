//
// Created by SeanOMik on 3/12/2021.
// Github: https://github.com/SeanOMik
//

#ifndef SIMPLEENGINE_DESTRUCTABLE_H
#define SIMPLEENGINE_DESTRUCTABLE_H

namespace simpleengine {
    class Destructable {
    public:
        virtual void destroy() {
            destroying = true;
        }

        virtual const bool& is_destroying() const {
            return destroying;
        }
    protected:
        bool destroying = false;
    };
}

#endif //SIMPLEENGINE_DESTRUCTABLE_H