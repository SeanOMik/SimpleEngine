//
// Created by SeanOMik on 7/6/2020.
// Github: https://github.com/SeanOMik
// Email: seanomik@gmail.com
//

#ifndef SIMPLEENGINE_RANDOM_H
#define SIMPLEENGINE_RANDOM_H

#include <limits>
#include <random>
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

        template<class Dist = std::uniform_int_distribution<>>
        T NextInRange(T min, T max) {
            Dist dist(min, max);

            return dist(gen);
        }
    };
}

#endif //SIMPLEENGINE_RANDOM_H