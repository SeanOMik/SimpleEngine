//
// Created by SeanOMik on 7/6/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_RANDOM_H
#define SIMPLEENGINE_RANDOM_H

#include <limits>
namespace simpleengine {
    namespace random {
        int RandomInt(int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max());
    }
}

#endif //SIMPLEENGINE_RANDOM_H