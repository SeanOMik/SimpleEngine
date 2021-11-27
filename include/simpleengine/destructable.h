//
// Created by SeanOMik on 3/12/2021.
// Github: https://github.com/SeanOMik
//

#pragma once

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