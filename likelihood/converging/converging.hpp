#ifndef CONVERGING_H
#define CONVERGING_H

#include "../../common/common.hpp"

#include <functional>

/*
Evaluates the log(sum over 0<=i<n, 0<=j<m: exp(terms(i, j)))
Assumes terms converge for increasing i, j
rel_accuracy should correspond approximately to proportional error on result - will be much more reliable for strictly positive terms
*/
scalar log_converging_double_sum(size_t n, size_t m, function<scalar(size_t i, size_t j)> terms, scalar rel_accuracy);

#endif