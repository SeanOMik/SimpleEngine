//
// Created by SeanOMik on 3/22/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_RANGE_H
#define SIMPLEENGINE_RANGE_H

#include <limits>

namespace simpleengine {
    template<typename T>
    class Range {
    public:
        Range() = default;
        Range(T min, T max) : min(min), max(max) {}
    
        T min = std::numeric_limits<T>::min();
        T max = std::numeric_limits<T>::max();
    };

    typedef Range<float> Rangef;
    typedef Range<int> Rangei;
}

#endif //SIMPLEENGINE_RANGE_H
