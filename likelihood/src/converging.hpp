#ifndef CONVERGING_H
#define CONVERGING_H

#include "core.hpp"
#include "caching/factorials.hpp"
#include "comparison.hpp"

#include <functional>

/*
Evaluates the log(sum over 0<=i<n, 0<=j<m: exp(terms(i, j)))
Assumes terms converge for increasing i, j
log_rel_accuracy should correspond approximately to proportional error on terms - will be much more reliable for strictly positive terms
*/
scalar log_converging_double_sum(size_t n, size_t m, std::function<scalar(size_t i, size_t j)> terms, scalar log_rel_accuracy);

scalar log_converging_bin_likelihood(FactorialCache cache, DetectorComparison comp, size_t count_1, size_t count_2, scalar log_accuracy);

scalar log_likelihood(FactorialCache cache, scalar background_rate_1, scalar background_rate_2, Histogram time_dist_1, Histogram time_dist_2, size_t n_bins, scalar log_accuracy);

#endif