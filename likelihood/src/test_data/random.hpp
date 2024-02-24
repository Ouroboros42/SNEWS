#ifndef RANDOM_H
#define RANDOM_H

#include "core.hpp"
#include <random>

/* Return a random real number in range [min, max] */
scalar rand_in_range(scalar min, scalar max);

/* Return a random integer in range [0, ceil) */
size_t rand_int(size_t ceil);

/* Return an observed count, distributed according to a Poissond distribution with the specified rate */
size_t poisson_trial(scalar rate);

#endif