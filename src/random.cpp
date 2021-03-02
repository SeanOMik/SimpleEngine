#include "random.h"

#include <random>

namespace simpleengine {
    int random::RandomInt(int min, int max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(min, max);

        return distr(gen);
    }
}