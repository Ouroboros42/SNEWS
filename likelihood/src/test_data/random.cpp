#include "random.hpp"

#include <random>

std::random_device rd;
std::mt19937_64 e1(rd());

size_t rand_int(size_t ceil) {
    std::uniform_int_distribution<size_t> range { 0, ceil };
    return range(e1);
}

scalar rand_in_range(scalar min, scalar max) {
    std::uniform_real_distribution<scalar> range { min, max };
    return range(e1);
}