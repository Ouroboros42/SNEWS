#ifndef RANDOM_H
#define RANDOM_H

#include "core.hpp"
#include <random>

// Random real number in range min to max
scalar rand_in_range(scalar min, scalar max);

// Random integer in range [0,ceil)
size_t rand_int(size_t ceil);

#endif