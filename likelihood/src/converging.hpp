#ifndef CONVERGING_H
#define CONVERGING_H

#include "core.hpp"

#include <functional>

/*
Evaluates the log(sum over 0<=i<n, 0<=j<m: exp(terms(i, j)))
Assumes terms converge for increasing i, j
log_rel_accuracy should correspond approximately to proportional error on terms - will be much more reliable for strictly positive terms
*/
scalar log_converging_double_sum(size_t n, size_t m, std::function<scalar(size_t i, size_t j)> terms, scalar log_rel_accuracy);

#endif