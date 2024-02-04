#include "terms.hpp"
#include "converging.hpp"

#include <iostream>

BinLikelihoodCache::BinLikelihoodCache(size_t max_count_1, size_t max_count_2, scalar detector_const_1, scalar detector_const_2)
: FactorialCache(max_count_1 + max_count_2),
max_count_1(max_count_1), max_count_2(max_count_2),
detector_const_1(detector_const_1), detector_const_2(detector_const_2),
series_1(exp_series(detector_const_1, max_count_1 + 1)), series_2(exp_series(detector_const_2, max_count_2 + 1)) {}

std::function<scalar(size_t i, size_t j)> BinLikelihoodCache::log_sum_terms(size_t count_1, size_t count_2) {
    return [this, count_1, count_2](size_t i, size_t j) {
        return series_1[i] + series_2[j] + log_binomial(count_1 - i, count_2 - j);
    };
}

scalar BinLikelihoodCache::log_converging_internal_likelihood(size_t count_1, size_t count_2, scalar log_rel_accuracy) {
    // Scale termwise accuracy to number of terms to achieve reliable overall accuracy
    return log_converging_double_sum(count_1, count_2, log_sum_terms(count_1, count_2), log_rel_accuracy - log(count_1) - log(count_2));
}