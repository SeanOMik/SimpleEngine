//
// Created by SeanOMik on 7/6/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_RANDOM_H
#define SIMPLEENGINE_RANDOM_H

#include <limits>
#include <random>
#include <type_traits>

namespace simpleengine {
    template<typename T, class RandomDevice = std::random_device, class Generator = std::mt19937>
    class Random {
    private:
        RandomDevice rd;
        Generator gen;
    public:
        Random() {
            this->gen = Generator(rd());
        }

        template<typename Dist = std::conditional_t<std::is_same_v<T, int> || std::is_same_v<T, uint32_t>,
            std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>>
        T NextInRange(T min, T max) {
            Dist dist(min, max);

            return dist(gen);
        }
    };
}

#endif //SIMPLEENGINE_RANDOM_H