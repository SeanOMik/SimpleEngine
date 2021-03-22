//
// Created by SeanOMik on 3/22/2021.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_RANGE_2_H
#define SIMPLEENGINE_RANGE_2_H

#include <limits>

namespace simpleengine {
    template<typename T>
    class Range2 {
    public:
        Range2() = default;
        Range2(T min_x, T max_x, T min_y, T max_y) : min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y) {}
    
        T min_x = std::numeric_limits<T>::min();
        T max_x = std::numeric_limits<T>::max();
        T min_y = std::numeric_limits<T>::min();
        T max_y = std::numeric_limits<T>::max();
    };

    typedef Range2<float> Range2f;
    typedef Range2<int> Range2i;
}

#endif //SIMPLEENGINE_RANGE_H
